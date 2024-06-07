#ifndef SKINDEF_H
#define SKINDEF_H
#ifdef __cplusplus
extern "C" {
#endif

#define JsonKey_BackgroundImage "bg"
#define JsonKey_ControlLabelImage "clb"
#define JsonKey_SettingLabelImage "slb"


#define JsonKey_Mode "mode"
#define JsonKey_State "state"
#define JsonKey_StatusTime "st"
#define JsonKey_BeerTemp "bt"
#define JsonKey_BeerSet "bs"
#define JsonKey_FridgeTemp "ft"
#define JsonKey_FridgeSet "fs"
#define JsonKey_RoomTemp "rt"
#define JsonKey_TemperatureUnit "tu"
#define JsonKey_RSSI "rx"
#define JsonKey_GDUpdate "lu"
#define JsonKey_GDTemperature "gt"
#define JsonKey_GDRSSI "grx"
#define JsonKey_GDAngle "tilt"
#define JsonKey_GDBattery "bat"
#define JsonKey_OriginalGravity "og"
#define JsonKey_ABV "abv"
#define JsonKey_ATT "att"
#define JsonKey_Gravity "sg"
#define JsonKey_Pressure "psi"
#define JsonKey_WirelessHydrometerName "hn"
#define JsonKey_StaticTexts "stxt"
#define JsonKey_StaticTextStyle "stxs"
#define JsonKey_ColorBlocks "cbs"

#define JsonKey_GlycolTemperature "glt"
#define JsonKey_GlycolTemperatureSet "glts"
#define JsonKey_GlycolState "gls"
#define JsonKey_GlycolElaspedTime "gle"

#define JsonKey_ChamberHumidity "hc"
#define JsonKey_RoomHumidity "hr"
#define JsonKey_TargetHumidity "ht"
#define JsonKey_HumidityControlState "hs"

#define JsonKey_ClickArea "click"
#define JsonKey_TemperatureControl "tc"
#define JsonKey_Settings "set"
#define JsonKey_InputGravit "ig"
#define JsonKey_InputOriginalGravity "iog"

#define JsonKey_GravityChange "gc"
#define JsonKey_GravityChange12H "gch"
#define JsonKey_GravityChange6H "gc6"
// properties
#define POS_x "x"
#define POS_y "y"
#define SIZE_w "w"
#define SIZE_h "h"
#define POS_ALIGNMENT "a"
#define TEXT_COLOR "tc"
#define TEXT_ALIGNMENT "ta"
#define TEXT_FONT "tf"
#define TIME_FORMAT_KEY "fmt"
#define BACKGROUND_COLOR "bg"
#define FILL_COLOR "fc"
#define SHADOW_COLOR "sc"
#define TEXT_DECORATION "td"

#define TEXT_DECORATION_UNDERLINE "u"
#define TEXT_DECORATION_STRIKETHROUGH "s"

#define POS_ALIGN_CENTER "c"
#define POS_ALIGN_TOPLEFT "tl"
#define POS_ALIGN_BOTTOMLEFT "bl"
#define POS_ALIGN_LEFTMID "lm"
#define POS_ALIGN_TOPRIGHT "tr"
#define POS_ALIGN_BOTTOMRIGHT "br"
#define POS_ALIGN_RIGHTMID "rm"

#define TEXT_ALIGN_CENTER "c"
#define TEXT_ALIGN_LEFT "l"
#define TEXT_ALIGN_RIGHT "r"

#define TEXT_FONT_SMALL "s"
#define TEXT_FONT_MEDIUM "m"
#define TEXT_FONT_LARGE "l"
#define TEXT_FONT_EXTRA_LARGE "x"
#define TEXT_FONT_CUSTOM1 "c1"
#define TEXT_FONT_CUSTOM2 "c2"
#define TEXT_FONT_CUSTOM3 "c3"

#define REPRESENTATION_TYPE "rep"

#define REPRESENTATION_ICON "ico"
#define REPRESENTATION_TEXT "txt"
#define REPRESENTATION_WIDGET "wgt"

#define TEXT_VALUE_LIST "texts"
#define ICON_PATH "icons"
#define OFFSET_LIST "off"
#define SIZE_AS_CONTENT "c"
#define STATIC_TEXT_VALUE "tx"

#define CORNER_RADUIS "r"
#define BORDER_WIDTH "bw"
#define BORDER_COLOR "bc"

typedef enum _SkinnableWidgetType{
    SkinnableWidgetTypeText,
    SkinnableWidgetTypeWidget,
    SkinnableWidgetTypeIcon
}SkinnableWidgetType;

typedef struct _IconOffset{
uint16_t x;
uint16_t y;
}IconOffset;


extern rxlevel_widget *rxlevel_ptr;
extern rxlevel_widget *gdRxlevel_ptr;
extern lv_obj_t *ui_lbWiFiRssi;
extern char* statusTimeFormat;
extern char* gravityDeviceUpdateTimeFormat;
extern char* glycolTimeFormat;
extern char** stateString;
extern char** modeString;
extern char** glycolStateString;
extern lv_obj_t *ui_imgMode;
extern lv_obj_t *ui_imgState;

extern lv_obj_t *ui_imgGlycolState;
#if EnableHumidityControlSupport
extern lv_obj_t *ui_imgHumidityControlState;
extern IconOffset* humidityControlStateIconOffsets;
extern char** humidityControlStateString;
#endif
//extern char* modeIconPath;
//extern char* stateIconPath;
extern IconOffset* modeIconOffsets;
extern IconOffset* stateIconOffsets;
extern IconOffset* glycolStateIconOffsets;

void createDefaultRxLevelWidget();

#ifdef __cplusplus
} //extern "C" {
#endif

#endif
