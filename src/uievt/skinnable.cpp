#include <string.h>
#include <stdlib.h>
#include "ArduinoJson.h"
#include "ui.h"
#include "rxlevel.h"
#include "skindef.h"
#include "BrewPiInterface.h"
#include "embedded_skin.h"

#define SKINFILENAME "A:/skin.json"

#ifndef SIMULATOR
#define DEBUGOUT(a) Serial.println(a)
#else
#define DEBUGOUT(a)
#endif

#define SystemFontSmall lv_font_montserrat_14
#define SystemFontMedium lv_font_montserrat_32
#define SystemFontLarge lv_font_montserrat_48
#define SystemFontExtraLarge Robo108

LV_FONT_DECLARE(Robo108);

lv_font_t *customFont_1;
lv_font_t *customFont_2;
lv_font_t *customFont_3;
const void* control_screen_label_image=NULL;
const void* setting_screen_label_image=NULL;

static void setDimension(lv_obj_t* obj,JsonVariant json,void (*setaction)(lv_obj_t*,lv_coord_t)){
    if(json.is<signed int>()){
        signed int val = json.as<signed int>();
            setaction( obj, val );
    }else if (json.is<const char*>()) {
        // content or percentage
        const char* str=json.as<const char*>();
        if(strcmp(str,SIZE_AS_CONTENT)==0){
            setaction( obj, LV_SIZE_CONTENT); 
        }else if(str[strlen(str)-1] == '%'){
            setaction(obj,lv_pct(atoi(str)));
        }
    }
}

static void assignCommonStyles(lv_obj_t *lvobj,const JsonObject& json){

    if(json.containsKey(POS_ALIGNMENT)){
        const char* align=json[POS_ALIGNMENT];
        if(strcmp(align,POS_ALIGN_CENTER)==0){
            lv_obj_set_align(lvobj,LV_ALIGN_CENTER);
        }else if(strcmp(align,POS_ALIGN_TOPLEFT)==0){
            lv_obj_set_align(lvobj,LV_ALIGN_TOP_LEFT);
        }else if(strcmp(align,POS_ALIGN_BOTTOMLEFT)==0){
            lv_obj_set_align(lvobj,LV_ALIGN_BOTTOM_LEFT);
        }else if(strcmp(align,POS_ALIGN_LEFTMID)==0){
            lv_obj_set_align(lvobj,LV_ALIGN_LEFT_MID);
        }else if(strcmp(align,POS_ALIGN_TOPRIGHT)==0){
            lv_obj_set_align(lvobj,LV_ALIGN_TOP_RIGHT);
        }else if(strcmp(align,POS_ALIGN_BOTTOMRIGHT)==0){
            lv_obj_set_align(lvobj,LV_ALIGN_BOTTOM_RIGHT);
        }else if(strcmp(align,POS_ALIGN_RIGHTMID)==0){
            lv_obj_set_align(lvobj,LV_ALIGN_RIGHT_MID);
        }
    }

    if(json.containsKey(POS_x)){
        signed int posx = json[POS_x];
        lv_obj_set_x( lvobj, posx );
    }    
    if(json.containsKey(POS_y)){
        signed int posy = json[POS_y];
        lv_obj_set_y( lvobj, posy );
    }    
    if(json.containsKey(SIZE_w)){
        setDimension(lvobj,json[SIZE_w],&lv_obj_set_width);
    }else{
        lv_obj_set_width( lvobj, LV_SIZE_CONTENT); 
    }
    if(json.containsKey(SIZE_h)){
        setDimension(lvobj,json[SIZE_h],&lv_obj_set_height);
    }else{
         lv_obj_set_height( lvobj, LV_SIZE_CONTENT); 
    } 
}

static void assignTextStyles(lv_obj_t *lvobj,const JsonObject& json){
    if(json.containsKey(TEXT_COLOR)){
        uint32_t color=strtol(json[TEXT_COLOR],NULL,16);
        lv_obj_set_style_text_color(lvobj, lv_color_hex(color), LV_PART_MAIN | LV_STATE_DEFAULT );
    }

    //lv_obj_set_style_text_opa(lvobj, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
    if(json.containsKey(TEXT_ALIGNMENT)){
        const char* ta=json[TEXT_ALIGNMENT].as<const char*>();
        if(strcmp(ta,TEXT_ALIGN_CENTER)==0){
            lv_obj_set_style_text_align(lvobj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN| LV_STATE_DEFAULT);
        }else  if(strcmp(ta,TEXT_ALIGN_LEFT)==0){
            lv_obj_set_style_text_align(lvobj, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN| LV_STATE_DEFAULT);
        }else  if(strcmp(ta,TEXT_ALIGN_RIGHT)==0){
            lv_obj_set_style_text_align(lvobj, LV_TEXT_ALIGN_RIGHT, LV_PART_MAIN| LV_STATE_DEFAULT);
        }
    }
    if(json.containsKey(TEXT_FONT)){
        const char* font=json[TEXT_FONT].as<const char*>();
        if(strcmp(font,TEXT_FONT_SMALL)==0){
            lv_obj_set_style_text_font(lvobj, & SystemFontSmall, LV_PART_MAIN| LV_STATE_DEFAULT);
        } else  if(strcmp(font,TEXT_FONT_MEDIUM)==0){
            lv_obj_set_style_text_font(lvobj, & SystemFontMedium, LV_PART_MAIN| LV_STATE_DEFAULT);
        } else  if(strcmp(font,TEXT_FONT_LARGE)==0){
            lv_obj_set_style_text_font(lvobj, & SystemFontLarge, LV_PART_MAIN| LV_STATE_DEFAULT);
        } else  if(strcmp(font,TEXT_FONT_EXTRA_LARGE)==0){
            lv_obj_set_style_text_font(lvobj, & SystemFontExtraLarge, LV_PART_MAIN| LV_STATE_DEFAULT);
        } else  if(strcmp(font,TEXT_FONT_CUSTOM1)==0 && customFont_1!=NULL){
            lv_obj_set_style_text_font(lvobj, customFont_1, LV_PART_MAIN| LV_STATE_DEFAULT);
        } else  if(strcmp(font,TEXT_FONT_CUSTOM2)==0 && customFont_2!=NULL){
            lv_obj_set_style_text_font(lvobj, customFont_2, LV_PART_MAIN| LV_STATE_DEFAULT);
        } else  if(strcmp(font,TEXT_FONT_CUSTOM3)==0 && customFont_3!=NULL){
            lv_obj_set_style_text_font(lvobj, customFont_3, LV_PART_MAIN| LV_STATE_DEFAULT);
        }
    }
}

static void constructTextItem(lv_obj_t **lvobj,const JsonObject& json){
    *lvobj=lv_label_create(ui_screenMain);
    assignCommonStyles(*lvobj,json);
    assignTextStyles(*lvobj,json);
}

static void constructRxLevel(lv_obj_t **lvobj,rxlevel_widget **rxwidget,const JsonObject& json){

    SkinnableWidgetType type=SkinnableWidgetTypeText;

    if(json.containsKey(REPRESENTATION_TYPE)){
        const char* rep=json[REPRESENTATION_TYPE];
        if(strcmp(rep,REPRESENTATION_WIDGET)==0){
            // create a widget
            type=SkinnableWidgetTypeWidget;
        }
        // otherwise, just assume text type
    }

    if(type == SkinnableWidgetTypeText){
        // text type
        *lvobj=lv_label_create(ui_screenMain);
        assignCommonStyles(*lvobj,json);
        assignTextStyles(*lvobj,json);
    }else{
        // widget.
        //
        uint16_t width=16,height=16;
        if(json.containsKey(SIZE_w)){
            width=json[SIZE_w];
        }
        if(json.containsKey(SIZE_h)){
            height=json[SIZE_h];
        }
        *rxwidget = lv_rxlevel_create(ui_screenMain,width,height);
        assignCommonStyles(lv_rxlevel_getCanvas(*rxwidget),json);

        if(json.containsKey(BACKGROUND_COLOR)){
            uint32_t color=strtol(json[BACKGROUND_COLOR],NULL,16);
            lv_rxlevel_setBgColor(*rxwidget, lv_color_hex(color) );
        }
        if(json.containsKey(FILL_COLOR)){
            uint32_t color=strtol(json[FILL_COLOR],NULL,16);
            lv_rxlevel_setColor(*rxwidget, lv_color_hex(color) );
        }
        if(json.containsKey(SHADOW_COLOR)){
            uint32_t color=strtol(json[SHADOW_COLOR],NULL,16);
            lv_rxlevel_setSubColor(*rxwidget, lv_color_hex(color) );
        }


    }
}

static void constructTimeWidget(lv_obj_t **lvobj,char** fmt_ptr,const JsonObject& json){
    *lvobj=lv_label_create(ui_screenMain);
    assignCommonStyles(*lvobj,json);
    assignTextStyles(*lvobj,json);
    // get time format
    if(json.containsKey(TIME_FORMAT_KEY)){
        const char* fmt=json[TIME_FORMAT_KEY];
        *fmt_ptr=strdup(fmt);
    }
}

//static void constructEnumTypeWidget(lv_obj_t **lvobj,char*** texts_ptr,lv_obj_t **iconobj,char** icons_ptr ,IconOffset** offsets_ptr,int requiredNumberOfItems,const JsonObject& json){
static void constructEnumTypeWidget(lv_obj_t **lvobj,char*** texts_ptr,lv_obj_t **iconobj ,IconOffset** offsets_ptr,int requiredNumberOfItems,const JsonObject& json){

    SkinnableWidgetType type=SkinnableWidgetTypeText;

    if(json.containsKey(REPRESENTATION_TYPE)){
        const char* rep=json[REPRESENTATION_TYPE];
        if(strcmp(rep,REPRESENTATION_ICON)==0){
            // create a widget
            type=SkinnableWidgetTypeIcon;
        }
        // otherwise, just assume text type
    }

    if(type == SkinnableWidgetTypeText){
        // text type.
        *lvobj=lv_label_create(ui_screenMain);
        assignCommonStyles(*lvobj,json);
        assignTextStyles(*lvobj,json);
        // get customize text, if any
        if(json.containsKey(TEXT_VALUE_LIST)){
            JsonArray texts = json[TEXT_VALUE_LIST].as<JsonArray>();
            if(requiredNumberOfItems == texts.size()){
                *texts_ptr=(char**)malloc(sizeof(char*) * requiredNumberOfItems);
                int i=0;
                for(JsonVariant v : texts) {
                    const char *t=v.as<const char*>();
                    (*texts_ptr)[i] = strdup(t);
                    i++;
                }
            }
        }
    }else{
        // icon type
        // load in advanced? or merge with text type?

        *iconobj = lv_img_create(ui_screenMain);
        assignCommonStyles(*iconobj,json);
        
        if(json.containsKey(ICON_PATH)){
        //  *icons_ptr = strdup(json[ICON_PATH].as<const char*>());
        //    lv_img_set_src(*iconobj,*icons_ptr);
            const char* src=json[ICON_PATH].as<const char*>();
            const lv_img_dsc_t *img=getEmbeddedImage(src);
            if(img){
                lv_img_set_src(*iconobj,img);
            }else{
                lv_img_set_src(*iconobj,strdup(src));
            }
        }

        if(json.containsKey(OFFSET_LIST)){

            JsonArray offsets = json[OFFSET_LIST].as<JsonArray>();

            if(requiredNumberOfItems * 2 == offsets.size()  ){
                IconOffset* intoffsets=(IconOffset*)malloc(sizeof(IconOffset) * requiredNumberOfItems);
                *offsets_ptr = intoffsets;
                for(int i=0;i<requiredNumberOfItems;i++) {
                    intoffsets[i].x=offsets[i*2].as<unsigned int>();
                    intoffsets[i].y=offsets[i*2 +1].as<unsigned int>();
                }
            }
        }
    }

}


static void constructTransparentButton(lv_obj_t **lvobj,void (*evcb)(lv_event_t*),const JsonObject& json){
    *lvobj = lv_btn_create(ui_screenMain);

    assignCommonStyles(*lvobj,json);

    lv_obj_add_flag( *lvobj, LV_OBJ_FLAG_SCROLL_ON_FOCUS );   /// Flags
    lv_obj_clear_flag( *lvobj, LV_OBJ_FLAG_SCROLLABLE );    /// Flags
    lv_obj_set_style_bg_color(*lvobj, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT );
    lv_obj_set_style_bg_opa(*lvobj, 0, LV_PART_MAIN| LV_STATE_DEFAULT);
    lv_obj_add_event_cb(*lvobj, evcb, LV_EVENT_ALL, NULL);
    //lv_state_t state= lv_obj_get_state(*lvobj);
    lv_obj_clear_state(*lvobj,LV_STATE_FOCUS_KEY | LV_STATE_FOCUSED);
}

static void setupStaticTextLabelStyle(const JsonObject& json,lv_style_t *style){
    lv_style_init(style);
    // text color, font, and decoration
    if(json.containsKey(TEXT_COLOR)){
        uint32_t color=strtol(json[TEXT_COLOR],NULL,16);
        lv_style_set_text_color(style, lv_color_hex(color));
    }

    if(json.containsKey(TEXT_DECORATION)){
        const char* td=json[TEXT_DECORATION].as<const char*>();
        if(strcmp(td,TEXT_DECORATION_UNDERLINE)==0){
            lv_style_set_text_decor(style, LV_TEXT_DECOR_UNDERLINE);
        }else  if(strcmp(td,TEXT_DECORATION_STRIKETHROUGH)==0){
            lv_style_set_text_decor(style, LV_TEXT_DECOR_STRIKETHROUGH);
        }
    }
    if(json.containsKey(TEXT_FONT)){
        const char* font=json[TEXT_FONT].as<const char*>();
        if(strcmp(font,TEXT_FONT_SMALL)==0){
            lv_style_set_text_font(style, & SystemFontSmall);
        } else  if(strcmp(font,TEXT_FONT_MEDIUM)==0){
            lv_style_set_text_font(style, & SystemFontMedium);
        } else  if(strcmp(font,TEXT_FONT_LARGE)==0){
            lv_style_set_text_font(style, & SystemFontLarge);
        } else  if(strcmp(font,TEXT_FONT_EXTRA_LARGE)==0){
            lv_style_set_text_font(style, & SystemFontExtraLarge);
        } else  if(strcmp(font,TEXT_FONT_CUSTOM1)==0 && customFont_1!=NULL){
            lv_style_set_text_font(style, customFont_1);
        } else  if(strcmp(font,TEXT_FONT_CUSTOM2)==0 && customFont_2!=NULL){
            lv_style_set_text_font(style, customFont_2);
        } else  if(strcmp(font,TEXT_FONT_CUSTOM3)==0 && customFont_3!=NULL){
            lv_style_set_text_font(style, customFont_3);
        }
    }
}

static lv_obj_t * constructStaticTextLabel(const JsonObject& json,lv_style_t *style){
    lv_obj_t *lvobj=lv_label_create(ui_screenMain);
    if(style){
        lv_obj_add_style(lvobj,style,LV_PART_MAIN | LV_STATE_DEFAULT);
    }

    assignCommonStyles(lvobj,json);
    assignTextStyles(lvobj,json);
    lv_label_set_text(lvobj,json[STATIC_TEXT_VALUE].as<const char*>());
    return lvobj;
}

static lv_obj_t *constructRectangleBlock(const JsonObject& json){
    lv_obj_t *lvobj=lv_obj_create(ui_screenMain);
    

    if(json.containsKey(FILL_COLOR)){
        uint32_t color=strtol(json[FILL_COLOR],NULL,16);
        lv_obj_set_style_bg_color(lvobj,lv_color_hex(color),0);
    }else{
        lv_obj_set_style_bg_opa(lvobj,LV_OPA_0,0);
    }
    if(json.containsKey(CORNER_RADUIS)){
        signed int radius = json[CORNER_RADUIS];
        lv_obj_set_style_radius(lvobj, radius,0);
    }else{
        lv_obj_set_style_radius(lvobj, 1,0);

    }

    if(json.containsKey(BORDER_WIDTH)){
        signed int border = json[BORDER_WIDTH];
        lv_obj_set_style_border_width(lvobj, border,0);
        
        if(json.containsKey(BORDER_COLOR)){
            uint32_t color=strtol(json[BORDER_COLOR],NULL,16);
            lv_obj_set_style_border_color(lvobj, lv_color_hex(color),0);
        }
    }else{
        lv_obj_set_style_border_width(lvobj, 0,0);
    }

    assignCommonStyles(lvobj,json);
    return lvobj;
}
typedef struct _LabelItem{
    const char *key;
    lv_obj_t **widget;
}LabelItem;
static const LabelItem labelItems[]={
   {JsonKey_BeerTemp,&ui_lbBeer},
   {JsonKey_BeerSet,&ui_lbBeerSet},
   {JsonKey_FridgeTemp,&ui_lbFridge},
   {JsonKey_FridgeSet,&ui_lbFridgeSet},
   {JsonKey_RoomTemp,&ui_lbRoom},
   {JsonKey_TemperatureUnit,&ui_lbDegree},
   {JsonKey_GDTemperature,&ui_lbISpindelTemp},
   {JsonKey_GDAngle,&ui_lbAngle},
   {JsonKey_GDBattery,&ui_lbISpindelBat},
   {JsonKey_OriginalGravity,&ui_lbOg},
   {JsonKey_ABV,&ui_lbAbv},
   {JsonKey_ATT,&ui_lbAtt},
   {JsonKey_Gravity,&ui_lbGravity},
   {JsonKey_Pressure,&ui_lbPressure},
   {JsonKey_WirelessHydrometerName,&ui_lbWirelessHydrometer},
   {JsonKey_GlycolTemperature,&ui_lbGlycolTemperature},
   {JsonKey_GlycolTemperatureSet,&ui_lbGlycolTempSet},
   {JsonKey_GravityChange,&ui_lbGravityChange},
   {JsonKey_GravityChange12H,&ui_lbGravityChange12H},
   {JsonKey_GravityChange6H,&ui_lbGravityChange6H}
#if EnableHumidityControlSupport
    ,
    {JsonKey_ChamberHumidity,&ui_lbChamberHumidity},
    {JsonKey_RoomHumidity,&ui_lbRoomHumidity},
    {JsonKey_TargetHumidity,&ui_lbTargetHumidity}
#endif

};

typedef struct _EnumItem{
    const char *key;
    lv_obj_t **labelWidget;
    lv_obj_t **imageWidget;
    char ***stringlist_p;
    IconOffset **iconoff;
    uint8_t    numberOfValue;
}EnumItem;
static const EnumItem enumItems[]={
    {JsonKey_Mode,&ui_lbMode,&ui_imgMode,&modeString,&modeIconOffsets,5},
    {JsonKey_State,&ui_lbState,&ui_imgState,&stateString,&stateIconOffsets,11},
    {JsonKey_GlycolState,&ui_lbGlycolState,&ui_imgGlycolState,&glycolStateString,&glycolStateIconOffsets,3}
#if EnableHumidityControlSupport
    ,{JsonKey_HumidityControlState,&ui_lbHumidityControlState,&ui_imgHumidityControlState,&humidityControlStateString,&humidityControlStateIconOffsets,4}
#endif
};
typedef struct _TimeItem{
    const char *key;
    lv_obj_t **labelWidget;
    char **format;
}TimeItem;

static const TimeItem timeItems[]={
{JsonKey_GDUpdate,&ui_lbUpdate,&gravityDeviceUpdateTimeFormat},
{JsonKey_StatusTime,&ui_lbStatusTime,&statusTimeFormat},
{JsonKey_GlycolElaspedTime,&ui_lbGlycolElapsedTime,&glycolTimeFormat}
};

typedef struct _ClickArea{
    const char *key;
    lv_obj_t **lvobj;
    void (*evcb)(lv_event_t*);
}ClickArea;

static const ClickArea clickAreas[]={
{JsonKey_TemperatureControl,&ui_btnControl,&ui_event_btnControl},
{JsonKey_Settings,&ui_btnSetting,&ui_event_btnSetting},
{JsonKey_InputGravit,&ui_btnGravity,&ui_event_btnGravity},
{JsonKey_InputOriginalGravity,&ui_btnOriginalGravity,&ui_event_btnOriginalGravity}
};

static bool skinMainScreen(char* data){

    //StaticJsonDocument<6400> root;
    DynamicJsonDocument root(6400);
	auto jsonerror=deserializeJson(root,data);

	if(jsonerror){
        // use default skin
         DEBUGOUT("parse Skin error.\n");

  		return false;
	}
    // render the display in the following order
    // 1. create screen
    // 2. Background Image
    // 3. Drawing
    // 4. Custom Font
    // 5. Static Texts
    // 6. Widgets.
    // 7. Touch Area

    //1. create screen
    ui_screenMain = lv_obj_create(NULL);
    lv_obj_clear_flag( ui_screenMain, LV_OBJ_FLAG_SCROLLABLE );    /// Flags

    //2. backgraound image.
    if(root.containsKey(JsonKey_BackgroundImage)){
        const char *imgsrc=root[JsonKey_BackgroundImage].as<const char*>();
        const lv_img_dsc_t *img=getEmbeddedImage(imgsrc);
        if(img){
            lv_obj_set_style_bg_img_src( ui_screenMain, img, LV_PART_MAIN | LV_STATE_DEFAULT );
        }else{
            //if image src is assigned to "file", the image would not be cached. 
            lv_obj_t *nimg=lv_img_create(ui_screenMain);
            lv_img_set_src(nimg,strdup(imgsrc));
            lv_obj_set_style_bg_img_src( ui_screenMain, img, LV_PART_MAIN | LV_STATE_DEFAULT );
        }
    }else{
        lv_obj_set_style_bg_color(ui_screenMain,lv_color_black(),0);
    }
    
    // control screen
    if(root.containsKey(JsonKey_ControlLabelImage)){
        const char *imgsrc=root[JsonKey_ControlLabelImage].as<const char*>();
        const lv_img_dsc_t *img=getEmbeddedImage(imgsrc);
        if(img){
            control_screen_label_image=img;
        }else{
            control_screen_label_image=strdup(imgsrc);
        }
    }

    // setting screen
    if(root.containsKey(JsonKey_SettingLabelImage)){
        const char *imgsrc=root[JsonKey_SettingLabelImage].as<const char*>();
        const lv_img_dsc_t *img=getEmbeddedImage(imgsrc);
        if(img){
            setting_screen_label_image=img;
        }else{
            setting_screen_label_image=strdup(imgsrc);
        }
    }
    // 3. drawing
    if(root.containsKey(JsonKey_ColorBlocks)){
            JsonArray blocks = root[JsonKey_ColorBlocks].as<JsonArray>();
            int i=0;
            for(JsonVariant v : blocks) {
                constructRectangleBlock(v);
                i++;
            }
    }
    //4. custom font
    if(root.containsKey(TEXT_FONT_CUSTOM1)){
        const char *src=root[TEXT_FONT_CUSTOM1].as<const char*>();
        customFont_1 =   lv_font_load(src);
    }
    if(root.containsKey(TEXT_FONT_CUSTOM2)){
        const char *src=root[TEXT_FONT_CUSTOM2].as<const char*>();
        customFont_2 =   lv_font_load(src);
    }
    if(root.containsKey(TEXT_FONT_CUSTOM3)){
        const char *src=root[TEXT_FONT_CUSTOM3].as<const char*>();
        customFont_3 =   lv_font_load(src);
    }


    //5. static texts
    if(root.containsKey(JsonKey_StaticTexts)){

            static lv_obj_t** staticTextsArray;
            static lv_style_t staticTextStyle;
            
            lv_style_t *style=NULL;

            if(root.containsKey(JsonKey_StaticTextStyle)){
                setupStaticTextLabelStyle(root[JsonKey_StaticTextStyle],&staticTextStyle);
                style= &staticTextStyle;
            }

            JsonArray texts = root[JsonKey_StaticTexts].as<JsonArray>();
            staticTextsArray = (lv_obj_t**) malloc(texts.size() * sizeof(lv_obj_t*));
            memset(staticTextsArray,0,texts.size() * sizeof(lv_obj_t*));

            int i=0;
            for(JsonVariant v : texts) {
                if(v.containsKey(STATIC_TEXT_VALUE)){
                    staticTextsArray[i] = constructStaticTextLabel(v,style);
                    i++;
                }               
            }
    }


    //6. widgets, items
    for(int idx=0;idx<sizeof(labelItems)/sizeof(LabelItem);idx++){
        if(root.containsKey(labelItems[idx].key)){
            constructTextItem(labelItems[idx].widget,root[labelItems[idx].key]);
        }
    }
    
    for(int idx=0;idx<sizeof(enumItems)/sizeof(EnumItem);idx++){
        if(root.containsKey(enumItems[idx].key)){
            constructEnumTypeWidget(enumItems[idx].labelWidget,enumItems[idx].stringlist_p,enumItems[idx].imageWidget
                ,enumItems[idx].iconoff,enumItems[idx].numberOfValue,root[enumItems[idx].key]);
       }
    }
    
    for(int idx=0;idx<sizeof(timeItems)/sizeof(TimeItem);idx++){
        if(root.containsKey(timeItems[idx].key)){
            constructTimeWidget(timeItems[idx].labelWidget,timeItems[idx].format,root[timeItems[idx].key]);
        }
    }

    if(root.containsKey(JsonKey_GDRSSI)){
        constructRxLevel(&ui_lbRssi,&gdRxlevel_ptr,root[JsonKey_GDRSSI]);
    }
    if(root.containsKey(JsonKey_RSSI)){
        constructRxLevel(&ui_lbWiFiRssi,&rxlevel_ptr,root[JsonKey_RSSI]);

    }
    //7.touch area 
    // to "overlap" buttons on all other widgets, create transparent buttons last
    if(root.containsKey(JsonKey_ClickArea)){
        JsonObject click=root[JsonKey_ClickArea];
        for(int idx=0;idx<sizeof(clickAreas)/sizeof(ClickArea);idx++){
            if(click.containsKey(clickAreas[idx].key)){
                constructTransparentButton(clickAreas[idx].lvobj,clickAreas[idx].evcb,click[clickAreas[idx].key]);
            }
        }
    }

    lv_obj_add_event_cb(ui_screenMain, ui_event_screenMain, LV_EVENT_ALL, NULL);

    DEBUGOUT("Skin successfully!!!!\n");

    return true;
}

#if SKINNABLE

#define MaxJsonFileSize 3096
void ui_screenMain_screen_init(void){
    bool skinned=false;
    char* buf=(char*)malloc(MaxJsonFileSize);
    uint32_t read_num;
    uint8_t skin=getSkin();
    if(skin>= NumberOfEmbeddedSkin){
        lv_fs_file_t f;
        lv_fs_res_t res = lv_fs_open(&f,SKINFILENAME, LV_FS_MODE_RD);
        if(res == LV_FS_RES_OK){
            res = lv_fs_read(&f, buf, MaxJsonFileSize, &read_num);

            if(res == LV_FS_RES_OK){
                buf[read_num]='\0';
                if(skinMainScreen(buf)){
                    skinned=true;
                }
                lv_fs_close(&f);
            }
        }else{
            DEBUGOUT("No Skin File.\n");
        }
        if(!skinned) skin=0; // set to default
    }
    if(skin<NumberOfEmbeddedSkin){
        strcpy(buf,getEmbeddedSkinFile(skin));
        skinMainScreen(buf);
    }
    free(buf);
}
#endif
