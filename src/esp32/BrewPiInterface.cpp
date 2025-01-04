
#if !SIMULATOR
#include <WiFi.h>
#include "Arduino.h"
#include "bpl/BrewPiProxy.h"
#include "bpl/WiFiSetup.h"
#include "BrewPiInterface.h"
#include "BPLSettings.h"

#include "ExternalData.h"
#include "PressureMonitor.h"
#include "WiFiSetup.h"
#include "TimeKeeper.h"
#include "ParasiteTempController.h"
#include "GravityTracker.h"

#if EnableHumidityControlSupport
#include "HumidityControl.h"
#endif

#if  HOMEKIT_UI
#include "Homekit.h"
#endif

#if SerialDebug == true
#define DebugPort Serial
#define DBG_PRINTF(...) DebugPort.printf(__VA_ARGS__)
#define DBG_PRINT(...) DebugPort.print(__VA_ARGS__)
#else
#define DBG_PRINTF(...)
#define DBG_PRINT(...)
#endif


uint8_t getSkin(void){
    return theSettings.systemConfiguration()->skin;
}

uint32_t getScreenSaverTime(void){
    return theSettings.systemConfiguration()->screenSaverTime  * 1000;
}

uint32_t getSleepTimeout(void){
    return theSettings.systemConfiguration()->sleepTimeout * 60000; // in minutes
}

/*****************************************************************/
/* brewpi related */
float BrewPiGetBeerTemp(){ 
    return brewPi.getBeerTemp();
}

float BrewPiGetBeerSet(){ 
    return brewPi.getBeerSet();
}

float BrewPiGetFridgeTemp(){
    return brewPi.getFridgeTemp();
}

float BrewPiGetFridgeSet(){
    return brewPi.getFridgeSet();
}

float BrewPiGetRoomTemp(){
    return brewPi.getRoomTemp();
}

char BrewPiGetUnit(){ 
    return brewPi.getUnit();
}

float BrewPiGetMinSetTemp(){
    return brewPi.getMinSetTemp();
}

float BrewPiGetMaxSetTemp(){
    return brewPi.getMaxSetTemp();
}

uint8_t BrewPiGetState(){
    return brewPi.getState();
}

uint8_t BrewPiGetMode(){
    char mode=brewPi.getMode();
    if(mode == 'o') return 0;
    if(mode == 'f') return 1;
    if(mode == 'b') return 2;
    if(mode == 'p') return 3;
    return 4;
}

uint32_t BrewPiGetStatusTime(){ 
    return brewPi.getStatusTime();
}
void BrewPiSetMode(uint8_t mode){
    static char modeMap[]={'o','f','b','p'};
	uint8_t idx= (mode>3)? 0:mode;
    brewPi.setMode(modeMap[idx]);
}

void BrewPiSetBeer(int temp){
	brewPi.setBeerSet((float)temp);
}

void BrewPiSetFridge(int temp){
	brewPi.setFridgetSet((float)temp);
}

/*****************************************************************/

float bplGetPressure(){
	return PressureMonitor.currentPsi();
}

/*****************************************************************/
/* network related*/
void bplConnectNetwork(const char* ssid,const char* pass,uint32_t ip,uint32_t gw,uint32_t nm,uint32_t dns){
    WiFiSetup.connect(ssid,pass,ip,gw,nm,dns);
}

uint8_t bplGetWiFiMode(){
	return theSettings.systemConfiguration()->wifiMode;//WiFi.getMode();
}
void bplSetWiFiMode(uint8_t newMode){
    theSettings.systemConfiguration()->wifiMode=(WiFiMode_t) newMode;
    WiFiSetup.setMode((WiFiMode_t) newMode);
}
int     bplGetWiFiRssi(){
    int rssi=WiFi.RSSI();
//    DBG_PRINTF("Get RSSI:%d\n",rssi);
	return rssi;
}

bool    bplIsWiFiConnected(){
	//DBG_PRINTF("wifi connected? %d\n",(WiFi.status() == WL_CONNECTED));
    return (WiFi.status() == WL_CONNECTED);
}

const char* bplGetSsid(){
    static char ssid[33];
	strncpy(ssid,WiFi.SSID().c_str(),33);
    return ssid;
}

uint32_t bplGetIpAddress(){
	return WiFi.localIP();
}

const char* bplHostname(){
	return theSettings.systemConfiguration()->hostnetworkname;
}

const char* bplUsername(){
	return theSettings.systemConfiguration()->username;
}

const char* bplPassword(){
	return theSettings.systemConfiguration()->password;
}

static void (*scan_result_cb)(WiFiListEntry* list,uint16_t count);

static void scanResultEntry(const char* ssid,int32_t rssi, bool enc, uint16_t index, uint16_t total){
    static WiFiListEntry* list=NULL;
    if(index ==0){
        if(total ==0){
            if(scan_result_cb)scan_result_cb(NULL,0);
            return;
        }
        // first one, alloc memory
        list=(WiFiListEntry*)malloc(sizeof(WiFiListEntry)* total);
    }
    WiFiListEntry* entry= list + index;
    strncpy(entry->ssid,ssid,32);
    entry->rssi = rssi;
    entry->enc = enc;
    if(index == (total -1)){
        // last one
        if(scan_result_cb)scan_result_cb(list,total);
    }
}

void bplRequestWiFiScan(void (*result_cb)(WiFiListEntry* list,uint16_t count) ){
    scan_result_cb=result_cb;
    if(! WiFiSetup.requestScanWifi(scanResultEntry)){
        if(scan_result_cb)scan_result_cb(NULL,0);
    }
}

uint32_t convertIp(const char* ip){
    IPAddress local_IP;
    local_IP.fromString(ip);
   return local_IP;
}
/*****************************************************************/
/* Gravity device*/

bool bplIsUsingPlato(){
	return  theSettings.GravityConfig()->usePlato;
}

float bplGravityDeviceTemperature(){
	return externalData.auxTemp();
}

float bplGravityDeviceTilt(){
	return externalData.tiltValue();
}

float bplGravityDeivceBattery(){
	return externalData.deviceVoltage();
}
char bplGravityDeviceBatteryUint(){
    return externalData.deviceBatteryUnit();
}

float bplGetGravity(){
	return theSettings.GravityConfig()->usePlato? externalData.plato():externalData.gravity();
}

float bplGetOriginalGravity(){
	return externalData.getOriginalGravity();
}

void bplSetGravity(float gravity){
	externalData.setGravity(gravity);
}

void bplSetOriginalGravity(float og){
	externalData.setOriginalGravity(og);
}

int   bplGravityDeviceRssi(){
	return externalData.rssi();
}

uint32_t bplGetGravityDeviceUpdateLocalTime(){
	return externalData.lastUpdate() + TimeKeeper.getTimezoneOffset();
}
extern const char *bplGetWirelessHydrometerName(){
    return externalData.getDeviceName();
}


//#if EanbleParasiteTempControl
// glycol(parasiteTemperature)

float bplGetGlycolTemperature(){
    return parasiteTempController.getTemperature();
}

float bplGetGlycolSetTemp(){
    return parasiteTempController.getSetTemperature();
}

uint8_t bplGetGlycolState(){
    char state = parasiteTempController.getMode();
    if(state == 'c') return 2;
    if(state == 'i') return 1;
    return 0;    
}
uint32_t bplGetGlycolElapsedTime(){
    return parasiteTempController.getTimeElapsed();
}
//#endif


float bplGetGravityDecreasedIn(int hour){
    int value=gravityTracker.gravityDecreasedIn(hour);
    if( value == InvalidGravityChangeValue){
        // return an invalid value
        return InvalidGravityDecreasedValue;
    }

    return ((float) value) /10.0;
}

#if EnableHumidityControlSupport
uint8_t bplGetRoomHumidity(){
    return humidityControl.roomHumidity();
}
uint8_t bplGetChamberHumidity(){ 
    return humidityControl.humidity();
}

uint8_t bplGetTargetHumidity(){
    return humidityControl.targetRH();
}
#endif

uint8_t bplGetHumidityControlState(){
    /*
        0: Off
        1: Idle
        2: Dehumiditying
        3: Humidifying
    */
   if(humidityControl.mode() == HC_ModeOff) return 0;
   HumidityControlState state = humidityControl.state();
   if(state == HC_Dehumidifying) return 2;
   if(state == HC_Humidifying) return 3;
   if(state == HC_Idle) return 1;
   return 0;
}

#if  HOMEKIT_UI

uint8_t bplHomekitGetStatus(void){
    return homekit_status();
}

void bplHomekitSetStatusCB(HomekitStatusIndicationCB cb){
    homekit_setup_status_cb(cb);
}

bool bplHomekitClearPairing(void){
    return homekit_reset_pairing();
}

bool bplHomekitStartPairing(void){
    homekit_restart_pairing();
    return true;
}

void bplHomekitGetSetupUri(char ret[]){
    homekit_get_setup_uri(ret);
}
const char* bplHomekitGetPairCode(void){
    return homekit_get_pair_code();
}

#endif

#endif