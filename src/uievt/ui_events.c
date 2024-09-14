// This file was generated by SquareLine Studio
// SquareLine Studio version: SquareLine Studio 1.3.2
// LVGL version: 8.3.6
// Project name: SquareLine_V1

#include "ui.h"
#include "BrewPiInterface.h"
#include "mystrlib.h"
#include "rxlevel.h"
#include "inputscreen.h"
#include "networkscan.h"
#include "screensaver.h"
#include "timeformat.h"
#include "skindef.h"

#define GRAVITY_TITLE "Gravity"
#define ORIGINAL_GRAVITY_TITLE "Original Gravity"


const char* defaultModeString[]={
"Off",
"Firdge Const.",
"Beer Const.",
"Beer Profile",
"Invalid"
};
const char* defaultStateString[]={
"Idling for", //	IDLE,						// 0
"Off", //	STATE_OFF,					// 1
"Door Open", //	DOOR_OPEN,					// 2 used by the Display only
"Heating for",	//HEATING,					// 3
"Cooling for",	//COOLING,					// 4
"Wait to Cool",	//WAITING_TO_COOL,			// 5
"Wait to Heat",	//WAITING_TO_HEAT,			// 6
"Wait for Peak", //	WAITING_FOR_PEAK_DETECT,	// 7
"Cool left", //	COOLING_MIN_TIME,			// 8
"Heat left", //	HEATING_MIN_TIME,			// 9
"Invalid"
};

const char* defaultGlycolStateString[]={
"Off",
"Idle",
"Cooling"
};

// the following 
rxlevel_widget *rxlevel_ptr;
rxlevel_widget *gdRxlevel_ptr;
// need to put it somewhereelse
lv_obj_t *ui_lbWiFiRssi;

const char DefaultStatusTimeFormat[]="%hh%mm%s";
const char DefaultGravityDeviceUpdateTimeFormat[]="%M/%d %h:%m";

char* statusTimeFormat=(char*)DefaultStatusTimeFormat;
char* gravityDeviceUpdateTimeFormat=(char*)DefaultGravityDeviceUpdateTimeFormat;
char* glycolTimeFormat=(char*)DefaultStatusTimeFormat;

// text type for Mode/State
char** stateString=(char**)defaultStateString;
char** modeString=(char**)defaultModeString;
char** glycolStateString=(char**)defaultGlycolStateString;
// for icon type of mode and state
lv_obj_t *ui_imgMode;
lv_obj_t *ui_imgState;
//char* modeIconPath;
//char* stateIconPath;
IconOffset* modeIconOffsets;
IconOffset* stateIconOffsets;

lv_obj_t *ui_imgGlycolState;
IconOffset* glycolStateIconOffsets;

#if EnableHumidityControlSupport
const char* defaultHumidityControlStateString[]={
"Off",
"Idle",
"Dehumidifying",
"Humidifying"
};


lv_obj_t *ui_imgHumidityControlState;
IconOffset* humidityControlStateIconOffsets;
char** humidityControlStateString=(char**) defaultHumidityControlStateString;
#endif

#define RxLevelWidth 21
#define RxLevelHeight 16

#define ValidPlato(p) ((p)>0 && (p)<100)
#define ValidSpecificGravity(sg)  ((sg)>0.1 && (sg)<2)

#define IS_FLOAT_TEMP_VALID(f) ((f)>-100.0)

enum BrewPiStates{
	IDLE,						// 0
	STATE_OFF,					// 1
	DOOR_OPEN,					// 2 used by the Display only
	HEATING,					// 3
	COOLING,					// 4
	WAITING_TO_COOL,			// 5
	WAITING_TO_HEAT,			// 6
	WAITING_FOR_PEAK_DETECT,	// 7
	COOLING_MIN_TIME,			// 8
	HEATING_MIN_TIME,			// 9
	INVALID_STATE
};
#define OffMode 0
#define	FridgeConstMode 1
#define	BeerConstMode 2
#define	BeerProfileMode 3
#define	InvalidMode 4



#define INVALID_FLOAT_TEMP -200

/* Main screen*/
void uiUpdateTemperature();
void uiUpdateMode();
void uiUpdateStatus();
void createDefaultRxLevelWidget();

void uiUpdateWiFiStatus();
void uiUpdateGravityDeviceWiFiStatus();
void uiUpdateWirelessHydrometerName();
void uiUpdateGravityDevice();
void uiUpdatePressure();
void uiUpdateGravityChange();
//#if	EanbleParasiteTempControl
void	uiUpdateParasiteTempControl();
//#endif

#if EnableHumidityControlSupport
void	uiUpdateHumidityControl();
#endif


/* Control screen */
void setArcValues(uint8_t mode);

/* input */
void originalGravityInput(const char* text);
void currentGravityInput(const char* text);


void screenSaverTimeout(lv_timer_t *timer);

void startScreenSaverTimer(void);

lv_obj_t* createWholeScreenButton();


lv_timer_t *screen_saver_timer;


/* ****************************************************************** */
/* called by BrewPiLess*/
/* ****************************************************************** */
void updateBrewPiInfo(void){
	if(isScreenSleeping()) return;
	if(lv_scr_act() != ui_screenMain) return;
	uiUpdateWiFiStatus();
	// mode
	uiUpdateMode();

	// temperature
	uiUpdateTemperature();
	// status line
	uiUpdateStatus();
	// pressure
	uiUpdatePressure();

//#if	EanbleParasiteTempControl
	uiUpdateParasiteTempControl();
//#endif
	uiUpdateGravityChange();
#if EnableHumidityControlSupport
	uiUpdateHumidityControl();
#endif

}

void updateGravityDevice(void){
	if(lv_scr_act() != ui_screenMain) return;
	// iSpindel info..
	uiUpdateGravityDevice();
}


/* ****************************************************************** */
/* Main screen*/
/* ****************************************************************** */

void createDefaultRxLevelWidget(){
	rxlevel_ptr = lv_rxlevel_create(ui_screenMain,RxLevelWidth,RxLevelHeight);
	// position and style
	lv_obj_t *canvas = lv_rxlevel_getCanvas(rxlevel_ptr);
	lv_obj_set_align(canvas,LV_ALIGN_TOP_RIGHT);
	lv_obj_set_x(canvas,2);
	lv_obj_set_y(canvas,1);
}

static void setTemperatureValue(lv_obj_t *widget,float value){
	char buffer[64];
	if(IS_FLOAT_TEMP_VALID((double)value)){
		sprintFloat(buffer,value,1);
		lv_label_set_text(widget,buffer);
	}
	else lv_label_set_text(widget,"--.-");
}

static void setGravityText(lv_obj_t *obj,float value,bool plato,bool isSG){
	char buffer[32];
	if(plato){
		if(ValidPlato(value)){
			sprintFloat(buffer,value,1);
			lv_label_set_text_fmt(obj,"%s°P",buffer);
		}else{
			lv_label_set_text(obj,"--");
		}
	}else{
		if(ValidSpecificGravity(value)){
			sprintFloat(buffer,value,isSG? 4:3);
			lv_label_set_text(obj,buffer);
		}else{
			lv_label_set_text(obj,"--");
		}
	}
}



void uiUpdateTemperature(){
	uint8_t mode= BrewPiGetMode();

	if(ui_lbBeer) setTemperatureValue(ui_lbBeer,BrewPiGetBeerTemp());
	if(ui_lbFridge) setTemperatureValue(ui_lbFridge,BrewPiGetFridgeTemp());
	if(ui_lbRoom) setTemperatureValue(ui_lbRoom,BrewPiGetRoomTemp());

	if(ui_lbBeerSet) setTemperatureValue(ui_lbBeerSet,(mode == BeerConstMode || mode== BeerProfileMode)? BrewPiGetBeerSet(): INVALID_FLOAT_TEMP);

	if(ui_lbFridgeSet) setTemperatureValue(ui_lbFridgeSet,(mode != OffMode)? BrewPiGetFridgeSet():INVALID_FLOAT_TEMP);
}

void uiUpdateMode(){
	uint8_t mode= BrewPiGetMode();
	if(mode > 3) mode=4;

	if(ui_lbMode){
		lv_label_set_text(ui_lbMode,modeString[mode]);
	}else if(ui_imgMode && modeIconOffsets){
		lv_img_set_offset_x(ui_imgMode,- modeIconOffsets[mode].x);
		lv_img_set_offset_y(ui_imgMode, -modeIconOffsets[mode].y);
	}
}


void uiUpdateStatus(){
	uint8_t state=BrewPiGetState();

	if(state >INVALID_STATE) state=INVALID_STATE;

	if(ui_lbState){
		lv_label_set_text(ui_lbState,stateString[state]);
	}else if(ui_imgState && stateIconOffsets){
		lv_img_set_offset_x(ui_imgState,- stateIconOffsets[state].x);
		lv_img_set_offset_y(ui_imgState, - stateIconOffsets[state].y);
	}

	if(ui_lbStatusTime &&(state == IDLE ||  state == HEATING || state == COOLING 
		|| state == WAITING_TO_COOL || state == WAITING_TO_HEAT 
		|| state == COOLING_MIN_TIME || state == HEATING_MIN_TIME)){
		uint32_t time  = BrewPiGetStatusTime();
		char buffer[64];
		formatTimePeriod(buffer,statusTimeFormat,time);
		lv_label_set_text(ui_lbStatusTime,buffer);
	}
}

static uint8_t getRssiLevel(int db){
    int strength[] ={-100,-80,-70,-67};
	int bar = 4;
    for (; bar > 0; bar--) {
        if (strength[bar] < db) break;
    }
	return (uint8_t)bar;
}

void uiUpdateWiFiStatus(){
	#if !SKINNABLE
	if(!rxlevel_ptr) createDefaultRxLevelWidget();
	#endif

	uint8_t wifimode = bplGetWiFiMode();

	if(ui_lbWiFiRssi){
		if(wifimode == WIFI_OFF || wifimode == WIFI_AP){
			lv_label_set_text(ui_lbWiFiRssi,"D");
		}else{
			if(bplIsWiFiConnected()){
				lv_label_set_text(ui_lbWiFiRssi,"?");
			}else{
				lv_label_set_text_fmt(ui_lbWiFiRssi,"%d",bplGetWiFiRssi());
			}
		}
	}else if (rxlevel_ptr){
		if(wifimode == WIFI_OFF || wifimode == WIFI_AP){
			lv_rxlevel_setDisabled(rxlevel_ptr,true);
		}else{
			uint8_t level;
			if(bplIsWiFiConnected()){
				int rssi =bplGetWiFiRssi();
				level=getRssiLevel(rssi);
			}else{
				level=0;
			}
			lv_rxlevel_setDisabled(rxlevel_ptr,false);
			lv_rxlevel_setLevel(rxlevel_ptr,level);
		}
	}
}

void uiUpdateGravityDeviceWiFiStatus(){
	int16_t rssi=bplGravityDeviceRssi();
	if(ui_lbRssi){
		if(IsRssiInvalid(rssi)){
			lv_label_set_text(ui_lbRssi,"--");
		}else{
			lv_label_set_text_fmt(ui_lbRssi,"%d",rssi);
		}
	}else if(gdRxlevel_ptr){
		if(IsRssiInvalid(rssi)){
			lv_rxlevel_setDisabled(rxlevel_ptr,false);
			lv_rxlevel_setLevel(rxlevel_ptr,0);
		}else{
			lv_rxlevel_setLevel(gdRxlevel_ptr,getRssiLevel(rssi));
		}
	}
}
void uiUpdateWirelessHydrometerName(){
	if(ui_lbWirelessHydrometer){
		lv_label_set_text(ui_lbWirelessHydrometer,bplGetWirelessHydrometerName());
	}
} 
void uiUpdateGravityDevice(){
	char buffer[64];
	// name
	uiUpdateWirelessHydrometerName();
	// temperature
	if(ui_lbISpindelTemp) setTemperatureValue(ui_lbISpindelTemp,bplGravityDeviceTemperature());

	// angle/tilt
	if(ui_lbAngle){
		float tilt=bplGravityDeviceTilt();
		if(tilt > 0){
			sprintFloat(buffer,tilt,2);
			lv_label_set_text_fmt(ui_lbAngle,"%s°",buffer);
		}else{
			lv_label_set_text(ui_lbAngle,"---");
		}
	}

	// battery
	if(ui_lbISpindelBat){
		float bat=bplGravityDeivceBattery();
		if(bat > 0){
			char unit=bplGravityDeviceBatteryUint();
			if(unit == '%'){
				int len=sprintInt(buffer,(int)bat);
				buffer[len]='\0';
			}else{
				sprintFloat(buffer,bat,2);
			}
			lv_label_set_text_fmt(ui_lbISpindelBat,"%s%c",buffer,unit);
		}else{
			lv_label_set_text(ui_lbISpindelBat,"---");
		}
	}
	//RSSI
	uiUpdateGravityDeviceWiFiStatus();

	// last update
	if(ui_lbUpdate){
		uint32_t last= bplGetGravityDeviceUpdateLocalTime();
		formatTime(buffer,gravityDeviceUpdateTimeFormat,last);
		lv_label_set_text(ui_lbUpdate,buffer);
	}

	// Gravity and OG
	bool plato = bplIsUsingPlato();
	float sg = bplGetGravity();
	float og = bplGetOriginalGravity();
	if(ui_lbGravity) setGravityText(ui_lbGravity,sg,plato,false);
	if(ui_lbOg) setGravityText(ui_lbOg,og,plato,false);
	// calculate ATT & ABV

	if((plato && !(ValidPlato(og) && ValidPlato(sg))) ||(!plato && !(ValidSpecificGravity(og) && ValidSpecificGravity(sg) ) )){
		if(ui_lbAbv) lv_label_set_text(ui_lbAbv,"--");
		if(ui_lbAtt) lv_label_set_text(ui_lbAtt,"--");
	}else{
		float att;
		float abv;
		if(plato){
			att = (og - sg)/og * 100.0;
			abv= (og - sg) / (2.0665 - (0.010665 * og));
		}else{
			att=(og-sg)/(og - 1.0)/100.0;
			abv = (og-sg) * 131.25;
		}
		if(ui_lbAbv){
			sprintFloat(buffer,abv,1);
			lv_label_set_text_fmt(ui_lbAbv,"%s%%",buffer);
		}
		if(ui_lbAtt){
			sprintFloat(buffer,att,1);
			lv_label_set_text_fmt(ui_lbAtt,"%s%%",buffer);
		}
	}

}

void uiUpdatePressure(){
	if(! ui_lbPressure) return;
	char buffer[32];
	float pressure = bplGetPressure();
	if(pressure >= 0){
		sprintFloat(buffer,pressure,1);
		lv_label_set_text(ui_lbPressure,buffer);
	}else{
		lv_label_set_text(ui_lbPressure,"--.-");
	}
}

static void setGravityChangeValue(lv_obj_t* obj,int hour){
	float value = bplGetGravityDecreasedIn(hour);
	if(IsGravityDecreasedValueValid(value)){
		//lv_label_set_text_fmt(obj,"%.1f",value);
		char buffer[32];
		sprintFloat(buffer,value,1);
		lv_label_set_text(obj,buffer);
	}else{
		lv_label_set_text(obj,"--");
	}
}

void uiUpdateGravityChange(){
	if(ui_lbGravityChange) setGravityChangeValue(ui_lbGravityChange,24);
	if(ui_lbGravityChange12H) setGravityChangeValue(ui_lbGravityChange12H,12);
	if(ui_lbGravityChange6H) setGravityChangeValue(ui_lbGravityChange6H,6);
}

//#if	EanbleParasiteTempControl
void	uiUpdateParasiteTempControl(){
	if(ui_lbGlycolTemperature) setTemperatureValue(ui_lbGlycolTemperature,bplGetGlycolTemperature());
	if(ui_lbGlycolTempSet) setTemperatureValue(ui_lbGlycolTempSet,bplGetGlycolSetTemp());
	// mode
	uint8_t state=bplGetGlycolState();

	if(ui_lbGlycolState){
		lv_label_set_text(ui_lbGlycolState,glycolStateString[state]);
	}else if(ui_imgGlycolState && glycolStateIconOffsets){
		lv_img_set_offset_x(ui_imgGlycolState,- glycolStateIconOffsets[state].x);
		lv_img_set_offset_y(ui_imgGlycolState, -glycolStateIconOffsets[state].y);
	}

	// time
	if(ui_lbGlycolElapsedTime){
		if(state ==0){
			lv_label_set_text(ui_lbGlycolElapsedTime,"");
		}else{
			uint32_t time  = bplGetGlycolElapsedTime();
			char buffer[64];
			formatTimePeriod(buffer,glycolTimeFormat,time);
			lv_label_set_text(ui_lbGlycolElapsedTime,buffer);
		}
	}
}
//#endif

#if EnableHumidityControlSupport
static void setHumidityValue(lv_obj_t *widget,uint8_t value){
	if(value>100) lv_label_set_text(widget,"--");
	else lv_label_set_text_fmt(widget,"%d%%",value);
} 
void uiUpdateHumidityControl(){
	if(ui_lbRoomHumidity) setHumidityValue(ui_lbRoomHumidity,bplGetRoomHumidity());
	if(ui_lbChamberHumidity) setHumidityValue(ui_lbChamberHumidity,bplGetChamberHumidity());
	if(ui_lbTargetHumidity) setHumidityValue(ui_lbTargetHumidity,bplGetTargetHumidity());
	uint8_t state=bplGetHumidityControlState();

	if(ui_lbHumidityControlState){
		lv_label_set_text(ui_lbHumidityControlState,humidityControlStateString[state]);
	}else if(ui_imgHumidityControlState && glycolStateIconOffsets){
		lv_img_set_offset_x(ui_imgHumidityControlState,- humidityControlStateIconOffsets[state].x);
		lv_img_set_offset_y(ui_imgHumidityControlState, -humidityControlStateIconOffsets[state].y);
	}
}
#endif


void userStopSaver(void){
	startScreenSaverTimer();
}

// called from BPL web request
void updateScreenSaverTime(void){

	if(lv_scr_act() == ui_screenMain){
		if(getScreenSaverTime()){
			lv_timer_set_period(screen_saver_timer,getScreenSaverTime());
			lv_timer_reset(screen_saver_timer);
			lv_timer_resume(screen_saver_timer);
		}else{
			lv_timer_pause(screen_saver_timer);
		}

	}
	// if screen saver is running, just wait until it returns?
}

void startScreenSaverTimer(void){
	uint32_t saver=getScreenSaverTime();
	uint32_t sleep= getSleepTimeout();
	if(saver || sleep){
		uint32_t timeout= (saver==0)? sleep:saver; 
		if(screen_saver_timer == NULL) screen_saver_timer= lv_timer_create(screenSaverTimeout,timeout,NULL);
		lv_timer_reset(screen_saver_timer);
		lv_timer_resume(screen_saver_timer);
	}
}

void onMainScreenLoadStart(lv_event_t * e)
{
	uiUpdateWiFiStatus();
	// mode
	uiUpdateMode();
	// temperature unit
	if(ui_lbDegree) lv_label_set_text_fmt(ui_lbDegree,"°%c",BrewPiGetUnit());

	// temperature
	uiUpdateTemperature();
	// status line
	uiUpdateStatus();
	// pressure
	uiUpdatePressure();
	// iSpindel info..
	uiUpdateGravityDevice();

//#if	EanbleParasiteTempControl
	uiUpdateParasiteTempControl();
//#endif
	uiUpdateGravityChange();
#if EnableHumidityControlSupport
	uiUpdateHumidityControl();
#endif


	// create screen saver
	startScreenSaverTimer();
}

void screenSaverTimeout(lv_timer_t *timer){
	lv_timer_pause(timer);
	screenSaver_start(ui_screenMain);
}

void onMainScreenHidden(lv_event_t * e){
	if(screen_saver_timer) lv_timer_pause(screen_saver_timer);
}


/* ****************************************************************** */
/* Gravity Editing*/
/* ****************************************************************** */

void fromEditor2Main(){
    _ui_screen_change(&ui_screenMain, LV_SCR_LOAD_ANIM_MOVE_BOTTOM, 300, 0, &ui_screenMain_screen_init);
}

void originalGravityInput(const char* text){
	bplSetOriginalGravity((float)atof(text));
	fromEditor2Main();
}

void currentGravityInput(const char* text){
	bplSetGravity((float)atof(text));
	fromEditor2Main();
}


void openEditGravity(lv_event_t * e)
{
	// Your code here
	openInputScreen(LV_SCR_LOAD_ANIM_MOVE_TOP, GRAVITY_TITLE,& currentGravityInput,&fromEditor2Main,InputTypeNumber,"");
}

void openEditOriginalGravity(lv_event_t * e)
{
	// Your code here
	openInputScreen(LV_SCR_LOAD_ANIM_MOVE_TOP,ORIGINAL_GRAVITY_TITLE,& originalGravityInput,&fromEditor2Main,InputTypeNumber,"");
}

/* ****************************************************************** */
/* control screen*/
/* ****************************************************************** */
void setArcValues(uint8_t mode){

	if(mode != BeerConstMode && mode !=FridgeConstMode){
		lv_obj_add_state(ui_arcTemperatureSet,LV_STATE_DISABLED);
		lv_label_set_text(ui_lbTemperatureSet,"--");
	}else{
		int value;
		if(mode == BeerConstMode){
			value = (int)BrewPiGetBeerSet();
		}else{ //if(mode == FridgeConstMode){
			value=(int)BrewPiGetFridgeSet();
		}
		int min =(int) BrewPiGetMinSetTemp();
		int max =(int) BrewPiGetMaxSetTemp();
		if( (value > max) || (value < min)){
			value = (BrewPiGetUnit() == 'C')? 20:68;
		}

		lv_arc_set_value(ui_arcTemperatureSet,value);
		lv_label_set_text_fmt(ui_lbTemperatureSet,"%d",value);
		lv_obj_clear_state(ui_arcTemperatureSet,LV_STATE_DISABLED);
	}

}

void onScreenTempControlLoadStart(lv_event_t * e)
{
	// temperature unit
	lv_label_set_text_fmt(ui_lbControlUnit,"°%c",BrewPiGetUnit());

	uint8_t mode= BrewPiGetMode();
	if(mode >3) mode=0;
	lv_slider_set_value(ui_sldMode,mode,LV_ANIM_OFF);
	// min, max.
	int min =(int) BrewPiGetMinSetTemp();
	int max =(int) BrewPiGetMaxSetTemp();
	lv_label_set_text_fmt(ui_lbMinTempSet,"%d",min);
	lv_label_set_text_fmt(ui_lbMaxTempSet,"%d",max);
	lv_arc_set_range(ui_arcTemperatureSet,min,max);

	setArcValues(mode);
}

void onModeChanged(lv_event_t * e)
{
	int mode=  lv_slider_get_value(ui_sldMode);
	setArcValues(mode);
	BrewPiSetMode(mode);
}

void onTemperatureSetDone(lv_event_t * e)
{
	uint8_t mode= BrewPiGetMode();
	int value = lv_arc_get_value(ui_arcTemperatureSet);
	if(mode == BeerConstMode){
		BrewPiSetBeer(value);
	}else if(mode == FridgeConstMode){
		BrewPiSetFridge(value);
	}
}
/* ****************************************************************** */
/* Setting screen*/
/* ****************************************************************** */

#define STA_Enabled(m) ((m) == WIFI_AP_STA || (m)== WIFI_STA)
#define AP_Enabled(m) ((m) == WIFI_AP_STA || (m)== WIFI_AP)

void onScreenSettingLoadStart(lv_event_t * e)
{
	// WiFi Mode
	uint8_t mode=bplGetWiFiMode();
	if(STA_Enabled(mode)){
		lv_obj_add_state(ui_swWiFi, LV_STATE_CHECKED);
		lv_obj_clear_state(ui_btnSearchNetwork,LV_STATE_DISABLED);
	}else{
		lv_obj_clear_state(ui_swWiFi, LV_STATE_CHECKED);
		lv_obj_add_state(ui_btnSearchNetwork,LV_STATE_DISABLED);
	}

	if(AP_Enabled(mode)) lv_obj_add_state(ui_swApMode, LV_STATE_CHECKED);
	else lv_obj_clear_state(ui_swApMode, LV_STATE_CHECKED);
	//SSID & IPAddress
	if(STA_Enabled(mode)){
		lv_label_set_text(ui_lbNetwork,bplGetSsid());
	   	union _IP{
        	uint8_t bytes[4];  // IPv4 address
        	uint32_t dword;
    	} IP;
		IP.dword = bplGetIpAddress();
		lv_label_set_text_fmt(ui_lbAddress,"%d.%d.%d.%d",IP.bytes[0],IP.bytes[1],IP.bytes[2],IP.bytes[3]);
	}else{
		lv_label_set_text(ui_lbNetwork,"");
		lv_label_set_text(ui_lbAddress,"0.0.0.0");
	}
	// hostname, username, password
	lv_label_set_text(ui_lbHostname,bplHostname());
	lv_label_set_text(ui_lbUsername,bplUsername());
	lv_label_set_text(ui_lbPasswd,bplPassword());
}

void onWiFiStaModeChange(lv_event_t * e)
{
	uint8_t mode=bplGetWiFiMode();
	uint8_t newMode;
	if(lv_obj_has_state(ui_swWiFi, LV_STATE_CHECKED)){
		// switch ON
		newMode = (mode == WIFI_AP)? WIFI_AP_STA:WIFI_STA; 
		lv_obj_clear_state(ui_btnSearchNetwork,LV_STATE_DISABLED);
	}else{
		// siwtch OFF
		newMode = (mode == WIFI_AP_STA)? WIFI_AP:WIFI_OFF; 
		lv_obj_add_state(ui_btnSearchNetwork,LV_STATE_DISABLED);
	}
	bplSetWiFiMode(newMode);
}

void onWiFiApModeChange(lv_event_t * e)
{
	uint8_t mode=bplGetWiFiMode();
	uint8_t newMode;
	if(lv_obj_has_state(ui_swApMode, LV_STATE_CHECKED)){
		// switch ON
		newMode = (mode == WIFI_STA)? WIFI_AP_STA:WIFI_AP; 
	}else{
		// siwtch OFF
		newMode = (mode == WIFI_AP_STA)? WIFI_STA:WIFI_OFF; 
	}
	bplSetWiFiMode(newMode);
}

void onSearchNetworkClicked(lv_event_t * e)
{
	// transit to scan network

	 _ui_screen_change( &cui_screenNetworkScan, LV_SCR_LOAD_ANIM_MOVE_LEFT, 300, 0, &cui_screenNetworkScan_screen_init);
}

/* ****************************************************************** */
/* Input screen*/
/* ****************************************************************** */

void onInputEnter(lv_event_t * e)
{
	// Your code here
	finishedInput();
}

void onKeyboardCancel(lv_event_t * e)
{
	// Your code here
	// Your code here
	cancelInput();

}
void onKeyboardReadyPressed(lv_event_t * e)
{
	// Your code here
	finishedInput();
}
