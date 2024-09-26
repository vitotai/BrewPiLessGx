#include "ui.h"
#include "screensaver.h"
#include "canvasdraw.h"
#include "driver_if.h"
#include "BrewPiInterface.h"
#include "ui_config.h"

#define COLOR_INDEX_BACKGROUND 0
#define COLOR_INDEX_BUBBLE 1

void screenSaver_create(lv_obj_t *parent);
lv_obj_t *cui_canvasSaver;
lv_obj_t *cui_btnScreen;
lv_timer_t *updateTimer;

typedef struct _Bubble{
 int x;
 int y;
}Bubble;

Bubble bubbles[BubbleCount];

typedef enum _ScreenSavingState{
ScreenSavingBubbling,
ScreenSavingInformation,
ScreenSavingSleep
}ScreenSavingState;

static uint32_t timerCount;
static uint32_t sleepCounter;

static ScreenSavingState _screenState;
static bool isTftSleeping=false;

void screenSaver_start(lv_obj_t* parent);
void onScreenSaverClicked(lv_event_t * e);
void screenSaver_hide(void);
void screenSaver_show(void);

void periodic_timer_handler(lv_timer_t *timer);
void screenSaverInitDraw(void);
void screenSaverUpdate(void);

void delay_sleep(void *timer);
void clearBg(void);

lv_color_t SRMColor[] = {
    LV_COLOR_MAKE(0xFF,0xF8,0x97),
    LV_COLOR_MAKE(0xF6,0xC1,0x01), 
    LV_COLOR_MAKE(0xDF,0x8D,0x03), 
    LV_COLOR_MAKE(0xC9,0x6E,0x12), 
    LV_COLOR_MAKE(0x64,0x38,0x05), 
    LV_COLOR_MAKE(0x2e,0x1a,0x02), 
    LV_COLOR_MAKE(0x1f,0x11,0x01)
    };

bool isScreenSleeping(void){
    return isTftSleeping;
}

void screenSaver_create(lv_obj_t *parent){
    static lv_color_t *canvasBuffer;
    // create button    
    
    cui_btnScreen = lv_btn_create(parent);
    lv_obj_set_size(cui_btnScreen,lv_pct(100),lv_pct(100));
    lv_obj_clear_flag( cui_btnScreen, LV_OBJ_FLAG_SCROLLABLE );    /// Flags
//    lv_obj_set_style_bg_color(cui_btnScreen, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT );
    lv_obj_set_style_bg_opa(cui_btnScreen, 0, LV_PART_MAIN| LV_STATE_DEFAULT);
    
    lv_obj_set_style_shadow_width(cui_btnScreen, 0, LV_PART_MAIN| LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_spread(cui_btnScreen, 0, LV_PART_MAIN| LV_STATE_DEFAULT);

    
    /*Create a canvas*/

    uint16_t width = lv_obj_get_width(parent);
    uint16_t height =lv_obj_get_height(parent);

    canvasBuffer=(lv_color_t*) lv_mem_alloc(sizeof(lv_color_t) * LV_CANVAS_BUF_SIZE_INDEXED_1BIT(width, height));

    cui_canvasSaver = lv_canvas_create(parent);

    lv_canvas_set_buffer(cui_canvasSaver, canvasBuffer, width, height, LV_IMG_CF_INDEXED_1BIT);
    lv_canvas_set_palette(cui_canvasSaver,COLOR_INDEX_BACKGROUND,lv_color_make(0xFC,0xC2,0x03));
    lv_canvas_set_palette(cui_canvasSaver,COLOR_INDEX_BUBBLE,lv_color_white());

    lv_obj_set_size(cui_canvasSaver,lv_pct(100),lv_pct(100));

    lv_obj_add_event_cb(cui_btnScreen,onScreenSaverClicked,LV_EVENT_CLICKED,NULL);
    updateTimer=lv_timer_create(periodic_timer_handler,UPDATE_TIMER,NULL);
    lv_timer_set_repeat_count(updateTimer,-1);
    lv_timer_pause(updateTimer);

    screenSaver_hide();
}

void delay_sleep(void *timer){
	(void) timer;
	display_drv_sleep();
    isTftSleeping=true;
}

void periodic_timer_handler(lv_timer_t *timer){
    sleepCounter++;

    if(getSleepTimeout()){
        if(sleepCounter >= getSleepTimeout()/UPDATE_TIMER){
            lv_timer_pause(updateTimer);
            if(_screenState == ScreenSavingBubbling){
                lv_obj_add_flag(cui_canvasSaver,LV_OBJ_FLAG_HIDDEN);
            }
            _screenState = ScreenSavingSleep;
            lv_async_call(delay_sleep,NULL);
            return;
        }
    }

    timerCount --;
    if(timerCount == 0){
        if(_screenState == ScreenSavingBubbling){
            // show underneath screen
            lv_obj_add_flag(cui_canvasSaver,LV_OBJ_FLAG_HIDDEN);
            timerCount = MainScreenTime;
            _screenState = ScreenSavingInformation;
        }else{
            lv_obj_clear_flag(cui_canvasSaver,LV_OBJ_FLAG_HIDDEN);
            timerCount = BubbleTime;
            screenSaverInitDraw();
            _screenState = ScreenSavingBubbling;
        }
    }else
    if(_screenState == ScreenSavingBubbling){
        screenSaverUpdate();
    }
}

void screenSaver_start(lv_obj_t* parent){

    if(!cui_canvasSaver){
        screenSaver_create(parent);
    }
    if(getScreenSaverTime() == 0){
        lv_obj_clear_flag(cui_btnScreen,LV_OBJ_FLAG_HIDDEN);
        // sleep
         _screenState = ScreenSavingSleep;
        lv_async_call(delay_sleep,NULL);
        return;
    }

    sleepCounter = 0;
    screenSaver_show();
    screenSaverInitDraw();
    lv_timer_resume(updateTimer);    
    _screenState = ScreenSavingBubbling;
    timerCount=BubbleTime;
}

void onScreenSaverClicked(lv_event_t * e){
    screenWakeup();
}

void screenWakeup(void){
    if(_screenState == ScreenSavingSleep){
        display_drv_wakeup();
//        delay(10);
        isTftSleeping=false;
    } else if(_screenState == ScreenSavingBubbling){
        screenSaver_hide();
    }
    else{
        lv_timer_pause(updateTimer);
    }
    screenSaver_hide();
    userStopSaver();
}

void screenSaver_hide(void){
    lv_timer_pause(updateTimer);
    lv_obj_add_flag(cui_canvasSaver,LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(cui_btnScreen,LV_OBJ_FLAG_HIDDEN);
}

void screenSaver_show(void){
    lv_obj_clear_flag(cui_canvasSaver,LV_OBJ_FLAG_HIDDEN);
    lv_obj_clear_flag(cui_btnScreen,LV_OBJ_FLAG_HIDDEN);
}


void screenSaverInitDraw(void){
    int colorIdx=lv_rand(0,sizeof(SRMColor)/sizeof(lv_color_t)-1);

    lv_canvas_set_palette(cui_canvasSaver,COLOR_INDEX_BACKGROUND,SRMColor[colorIdx]);
    if(colorIdx < 3){
        lv_canvas_set_palette(cui_canvasSaver,COLOR_INDEX_BUBBLE,lv_color_black());
    }else{
        lv_canvas_set_palette(cui_canvasSaver,COLOR_INDEX_BUBBLE,lv_color_white());
    }

    clearBg();

    uint16_t width = lv_obj_get_width(ui_screenMain);
    uint16_t height = lv_obj_get_height(ui_screenMain);
    lv_color_t color;
    color.full= COLOR_INDEX_BUBBLE;


    for(int i=0;i<BubbleCount;i++){
        bubbles[i].x= lv_rand(0,width);
        bubbles[i].y= lv_rand(0,height);
        canvasDrawCircleSafe_nv(cui_canvasSaver,bubbles[i].x,bubbles[i].y,BubbleSize,color);
    }
    lv_obj_invalidate(cui_canvasSaver);
}

void screenSaverUpdate(void){
    clearBg();

    uint16_t width = lv_obj_get_width(cui_canvasSaver);
    uint16_t height = lv_obj_get_height(cui_canvasSaver);
    lv_color_t color;
    color.full= COLOR_INDEX_BUBBLE;


    for(int i=0;i<BubbleCount;i++){
        int x = (int) bubbles[i].x +  lv_rand(-BubbleMoveDistance,BubbleMoveDistance);
        int y = (int) bubbles[i].y - BubbleFloatStep;
        
        if(x < BubbleSize) x=BubbleSize;
        if(x + BubbleSize >  width) x= width- BubbleSize;
        if(y + BubbleSize < 0){
            y = height + BubbleSize -1;
            x =lv_rand(0,width);
        }
        bubbles[i].x=x;
        bubbles[i].y=y;

       canvasDrawCircleSafe_nv(cui_canvasSaver,x,y,BubbleSize,color);
    }
    lv_obj_invalidate(cui_canvasSaver);
}


void clearBg(void){
    lv_color_t c1;
    c1.full= COLOR_INDEX_BACKGROUND;
    lv_canvas_fill_bg(cui_canvasSaver,c1,0);
}

