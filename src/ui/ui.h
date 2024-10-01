// This file was generated by SquareLine Studio
// SquareLine Studio version: SquareLine Studio 1.3.2
// LVGL version: 8.3.6
// Project name: S320240

#ifndef _S320240_UI_H
#define _S320240_UI_H

#ifdef __cplusplus
extern "C" {
#endif

    #include "lvgl.h"

#include "ui_helpers.h"
#include "ui_events.h"
// SCREEN: ui_screenMain
#if SKINNABLE
void ui_screenMain_screen_init_codegen(void);
#endif

void ui_screenMain_screen_init(void);

void ui_event_screenMain( lv_event_t * e);
extern lv_obj_t *ui_screenMain;
extern lv_obj_t *ui_lbMode;
extern lv_obj_t *ui_lbState;
extern lv_obj_t *ui_lbStatusTime;
extern lv_obj_t *ui_lbBeer;
extern lv_obj_t *ui_lbBeerSet;
extern lv_obj_t *ui_lbRoom;
extern lv_obj_t *ui_lbFridge;
extern lv_obj_t *ui_lbFridgeSet;
extern lv_obj_t *ui_lbPressure;
extern lv_obj_t *ui_lbDegree;
extern lv_obj_t *ui_lbGravity;
extern lv_obj_t *ui_lbISpindelTemp;
extern lv_obj_t *ui_lbAngle;
extern lv_obj_t *ui_lbISpindelBat;
extern lv_obj_t *ui_lbOg;
extern lv_obj_t *ui_lbAbv;
extern lv_obj_t *ui_lbAtt;
extern lv_obj_t *ui_lbUpdate;
extern lv_obj_t *ui_lbRssi;
extern lv_obj_t *ui_lbWirelessHydrometer;

//#if	EanbleParasiteTempControl
extern lv_obj_t *ui_lbGlycolTemperature;
extern lv_obj_t *ui_lbGlycolTempSet;
extern lv_obj_t *ui_lbGlycolState;
extern lv_obj_t *ui_lbGlycolElapsedTime;
//#endif
#if EnableHumidityControlSupport
extern lv_obj_t *ui_lbHumidityControlState;
extern lv_obj_t *ui_lbRoomHumidity;
extern lv_obj_t *ui_lbChamberHumidity;
extern lv_obj_t *ui_lbTargetHumidity;
#endif

extern lv_obj_t *ui_lbGravityChange;
extern lv_obj_t *ui_lbGravityChange12H;
extern lv_obj_t *ui_lbGravityChange6H;

void ui_event_btnSetting( lv_event_t * e);
extern lv_obj_t *ui_btnSetting;
void ui_event_btnControl( lv_event_t * e);
extern lv_obj_t *ui_btnControl;
void ui_event_btnGravity( lv_event_t * e);
extern lv_obj_t *ui_btnGravity;
void ui_event_btnOriginalGravity( lv_event_t * e);
extern lv_obj_t *ui_btnOriginalGravity;
// SCREEN: ui_screenTempControl
void ui_screenTempControl_screen_init(void);
void ui_event_screenTempControl( lv_event_t * e);
extern lv_obj_t *ui_screenTempControl;
void ui_event_btnTempControlBack( lv_event_t * e);
extern lv_obj_t *ui_btnTempControlBack;
void ui_event_sldMode( lv_event_t * e);
extern lv_obj_t *ui_sldMode;
void ui_event_arcTemperatureSet( lv_event_t * e);
extern lv_obj_t *ui_arcTemperatureSet;
extern lv_obj_t *ui_lbMinTempSet;
extern lv_obj_t *ui_lbMaxTempSet;
extern lv_obj_t *ui_lbTemperatureSet;
extern lv_obj_t *ui_lbControlUnit;
extern lv_obj_t *ui_Image1;
// SCREEN: ui_screenSetting
void ui_screenSetting_screen_init(void);
void ui_event_screenSetting( lv_event_t * e);
extern lv_obj_t *ui_screenSetting;
void ui_event_btnSettingBack( lv_event_t * e);
extern lv_obj_t *ui_btnSettingBack;
void ui_event_swWiFi( lv_event_t * e);
extern lv_obj_t *ui_swWiFi;
extern lv_obj_t *ui_lbNetwork;
extern lv_obj_t *ui_lbAddress;
void ui_event_swApMode( lv_event_t * e);
extern lv_obj_t *ui_swApMode;
extern lv_obj_t *ui_lbHostname;
extern lv_obj_t *ui_lbUsername;
extern lv_obj_t *ui_lbPasswd;
void ui_event_btnSearchNetwork( lv_event_t * e);
extern lv_obj_t *ui_btnSearchNetwork;
extern lv_obj_t *ui_Image2;
// SCREEN: ui_screenInput
void ui_screenInput_screen_init(void);
extern lv_obj_t *ui_screenInput;
void ui_event_kbInput( lv_event_t * e);
extern lv_obj_t *ui_kbInput;
extern lv_obj_t *ui_lbInputLabel;
extern lv_obj_t *ui_taInput;
extern lv_obj_t *ui____initial_actions0;

LV_IMG_DECLARE( ui_img_main_320240v2_png);   // assets/main_320240v2.png
LV_IMG_DECLARE( ui_img_control_320240_png);   // assets/control_320240.png
LV_IMG_DECLARE( ui_img_control_320240_top_png);   // assets/control_320240.png
LV_IMG_DECLARE( ui_img_backbutton_png);   // assets/BackButton.png
LV_IMG_DECLARE( ui_img_setting_320240_png);   // assets/setting_320240.png
LV_IMG_DECLARE( ui_img_203665058);   // assets/search-button-32.png
LV_IMG_DECLARE( ui_img_setting_320240_left_png);   // assets/setting_320240_left.png
LV_IMG_DECLARE( ui_img_control_320240_png);   // assets/control_320240.png
LV_IMG_DECLARE( ui_img_cotrol_3_5_png);   // assets/cotrol_3.5.png
LV_IMG_DECLARE( ui_img_main3_5_png);   // assets/main3.5.png
LV_IMG_DECLARE( ui_img_setting_3_5_png);   // assets/setting_3.5.png
LV_IMG_DECLARE( ui_img_main_320240_png);   // assets/main_320240.png

LV_FONT_DECLARE( ui_font_Font1);

void ui_init(void);

extern const void* control_screen_label_image;
extern const void* setting_screen_label_image;

#define AUTO_BACK_TIMER 30000
#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif
