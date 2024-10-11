#include "ui.h"
#include "screensaver.h"
#include "canvasdraw.h"
#include "driver_if.h"
#include "BrewPiInterface.h"

#define UPDATE_TIMER 150
#define BubbleSize 20
#define BubbleFloatStep 8
#define BubbleMoveDistance 4
#define BubbleCount 10

#define BubbleTime (10000/UPDATE_TIMER)
#define MainScreenTime (5000/UPDATE_TIMER)

#define COLOR_INDEX_BACKGROUND 0
#define COLOR_INDEX_BUBBLE 1

void screenSaver_create(lv_obj_t *parent);
lv_obj_t *cui_saverBackground;
lv_obj_t *cui_btnScreen;
lv_timer_t *updateTimer;
lv_style_t bubbleStyle;
lv_obj_t* bubbleObject[BubbleCount];

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

lv_color_t SRMColor[] = {
    LV_COLOR_MAKE(0xFD,0xFE,0x47), // SRM 2, Pale Straw
    LV_COLOR_MAKE(0xFD,0xEA,0x3F), // sRM 3, Straw
    LV_COLOR_MAKE(0xFB,0xD9,0x49), // 4, Pale Gold
    LV_COLOR_MAKE(0xFA,0xA8,0x46), // 6, Deep Gold
    LV_COLOR_MAKE(0xF4,0xA0,0x45), // 9, Pale Amber
    LV_COLOR_MAKE(0xD7,0x80,0x5a), // 12, Mediam Amber
    LV_COLOR_MAKE(0x93,0x53,0x3c), // 15, Deep Ameber

    LV_COLOR_MAKE(0x80,0x46,0x43), // 18, Ameber brown
    LV_COLOR_MAKE(0x5D,0x36,0x31), // 20, brown
    LV_COLOR_MAKE(0x4E,0x3C,0x2D), // 24, Ruby Brown
    LV_COLOR_MAKE(0x3A,0x31,0x2E), // 30, Deep brown
    LV_COLOR_MAKE(0x32,0x31,0x2C) // 40, Black
    };

bool isScreenSleeping(void){
    return isTftSleeping;
}

void screenSaver_create(lv_obj_t *parent){
    // create button    
    
    cui_btnScreen = lv_btn_create(parent);
    lv_obj_set_size(cui_btnScreen,lv_pct(100),lv_pct(100));
    lv_obj_clear_flag( cui_btnScreen, LV_OBJ_FLAG_SCROLLABLE );    /// Flags
//    lv_obj_set_style_bg_color(cui_btnScreen, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT );
    lv_obj_set_style_bg_opa(cui_btnScreen, 0, LV_PART_MAIN| LV_STATE_DEFAULT);
    
    lv_obj_set_style_shadow_width(cui_btnScreen, 0, LV_PART_MAIN| LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_spread(cui_btnScreen, 0, LV_PART_MAIN| LV_STATE_DEFAULT);

    
    cui_saverBackground = lv_obj_create(parent);

    lv_obj_set_size(cui_saverBackground,lv_pct(100),lv_pct(100));
    lv_obj_clear_flag( cui_saverBackground, LV_OBJ_FLAG_SCROLLABLE );    /// Flags
    lv_obj_set_style_radius(cui_saverBackground,0,LV_PART_MAIN);
    lv_obj_set_style_border_width(cui_saverBackground,0,LV_PART_MAIN);
    // create bubbles

    // initialize style
    lv_style_init(&bubbleStyle);
    /*Set a background color and a radius*/
    lv_style_set_radius(&bubbleStyle, BubbleSize);
    lv_style_set_bg_opa(&bubbleStyle, LV_OPA_20);
    lv_style_set_bg_color(&bubbleStyle,lv_color_white());


    /*Add border to the bottom+right*/
    
    //lv_style_set_border_color(&bubbleStyle, lv_palette_main(LV_PALETTE_BLUE));
    lv_style_set_border_width(&bubbleStyle, 0);
    //lv_style_set_border_opa(&bubbleStyle, LV_OPA_50);
    //lv_style_set_border_side(&bubbleStyle, LV_BORDER_SIDE_FULL);
    

    for(int i=0;i<BubbleCount;i++){
        bubbleObject[i] = lv_obj_create(cui_saverBackground);
        lv_obj_add_style(bubbleObject[i],&bubbleStyle,LV_PART_MAIN);
        lv_obj_set_size(bubbleObject[i],BubbleSize*2,BubbleSize*2);
    }
    // create timer
    updateTimer=lv_timer_create(periodic_timer_handler,UPDATE_TIMER,NULL);
    lv_timer_set_repeat_count(updateTimer,-1);
    lv_timer_pause(updateTimer);

    // click event handling
    lv_obj_add_event_cb(cui_btnScreen,onScreenSaverClicked,LV_EVENT_CLICKED,NULL);
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
                lv_obj_add_flag(cui_saverBackground,LV_OBJ_FLAG_HIDDEN);
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
            lv_obj_add_flag(cui_saverBackground,LV_OBJ_FLAG_HIDDEN);
            timerCount = MainScreenTime;
            _screenState = ScreenSavingInformation;
        }else{
            lv_obj_clear_flag(cui_saverBackground,LV_OBJ_FLAG_HIDDEN);
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

    if(!cui_saverBackground){
        screenSaver_create(parent);
    }else{
        screenSaver_show();
    }
    if(0 == getScreenSaverTime()){
        lv_obj_clear_flag(cui_btnScreen,LV_OBJ_FLAG_HIDDEN);
        // sleep
         _screenState = ScreenSavingSleep;
        lv_async_call(delay_sleep,NULL);
        return;
    }

    sleepCounter = 0;
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
    }else{
        lv_timer_pause(updateTimer);
    }
    screenSaver_hide();
    userStopSaver();
}

void screenSaver_hide(void){
    lv_timer_pause(updateTimer);
    lv_obj_add_flag(cui_saverBackground,LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(cui_btnScreen,LV_OBJ_FLAG_HIDDEN);
}

void screenSaver_show(void){
    lv_obj_clear_flag(cui_saverBackground,LV_OBJ_FLAG_HIDDEN);
    lv_obj_clear_flag(cui_btnScreen,LV_OBJ_FLAG_HIDDEN);
}


void screenSaverInitDraw(void){
    int colorIdx=lv_rand(0,sizeof(SRMColor)/sizeof(lv_color_t)-1);

    lv_obj_set_style_bg_color(cui_saverBackground,SRMColor[colorIdx],LV_PART_MAIN);

    if(colorIdx < 3){
        lv_style_set_bg_opa(&bubbleStyle, LV_OPA_40);
    }else{
        lv_style_set_bg_opa(&bubbleStyle, LV_OPA_20);
    }

    uint16_t width =  lv_obj_get_width(ui_screenMain);
    uint16_t height = lv_obj_get_height(ui_screenMain);

    for(int i=0;i<BubbleCount;i++){
        int32_t x= lv_rand(0,width);
        int32_t y= lv_rand(0,height);
        lv_obj_set_pos(bubbleObject[i],x,y);
    }
}

void screenSaverUpdate(void){

    uint16_t width = lv_obj_get_width(cui_saverBackground);
    uint16_t height = lv_obj_get_height(cui_saverBackground);

    for(int i=0;i<BubbleCount;i++){
        lv_obj_t * bubble = bubbleObject[i];
        
        int x = lv_obj_get_x(bubble); 
        int y = lv_obj_get_y(bubble);

        x +=  lv_rand(0,BubbleMoveDistance*2)-BubbleMoveDistance;
        y -=  BubbleFloatStep;

        if(x < BubbleSize) x=BubbleSize;
        if(x + BubbleSize >  width) x= width- BubbleSize;
        if(y + BubbleSize < 0){
            y = height + BubbleSize -1;
            x =lv_rand(0,width);
        }
        lv_obj_set_pos(bubble,x,y);
    }
    lv_obj_invalidate(cui_saverBackground);
}

