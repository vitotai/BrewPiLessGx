#ifndef BrewPiInterface_H
#define BrewPiInterface_H

//#include <Arduino.h>
//#include "Config.h"
#ifdef __cplusplus
extern "C" {
#endif

#define IsRssiInvalid(r) ((r)<-200)
#define IsBatteryInvalid(b) ((b)<0)

extern uint8_t getSkin(void);
uint32_t getScreenSaverTime(void);

uint32_t getSleepTimeout(void);

void updateBrewPiInfo(void);
void updateGravityDevice(void);

extern uint32_t convertIp(const char* ip);

extern	float BrewPiGetBeerTemp(void);
extern	float BrewPiGetBeerSet(void);
extern	float BrewPiGetFridgeTemp(void);
extern	float BrewPiGetFridgeSet(void);
extern	float BrewPiGetRoomTemp(void);
extern	char BrewPiGetUnit(void);
extern	float BrewPiGetMinSetTemp(void);
extern	float BrewPiGetMaxSetTemp(void);
extern	uint8_t BrewPiGetState(void);
extern	uint8_t BrewPiGetMode(void);
extern uint32_t BrewPiGetStatusTime(void);

bool BrewPiAmbientSensorConnected(void);

extern void BrewPiSetMode(uint8_t mode);
extern void BrewPiSetBeer(int temp);
extern void BrewPiSetFridge(int temp);

// BPL

#ifndef WIFI_OFF
#define WIFI_OFF     0
#define WIFI_STA     1
#define WIFI_AP      2
#define WIFI_AP_STA  3
#endif

typedef struct _WiFiListEntry{
    char ssid[32];
    int32_t   rssi;
    bool  enc;
}WiFiListEntry;


extern uint8_t bplGetWiFiMode(void);
extern void bplSetWiFiMode(uint8_t newMode);
extern int     bplGetWiFiRssi(void);
extern bool    bplIsWiFiConnected(void);
extern void bplConnectNetwork(const char* ssid,const char* passphase,uint32_t ip,uint32_t gw,uint32_t nm,uint32_t dns);

extern const char* bplGetSsid(void);
extern uint32_t bplGetIpAddress(void);
extern const char* bplHostname(void);
extern const char* bplUsername(void);
extern const char* bplPassword(void);
extern void bplRequestWiFiScan(void (*result_cb)(WiFiListEntry* list,uint16_t count) );

extern float bplGetPressure(void);

extern bool bplIsUsingPlato(void);
extern float bplGravityDeviceTemperature(void);
extern float bplGravityDeviceTilt(void);
extern float bplGravityDeivceBattery(void);
extern char bplGravityDeviceBatteryUint(void);
extern int   bplGravityDeviceRssi(void);
extern float bplGetGravity(void);
extern float bplGetOriginalGravity(void);
extern uint32_t bplGetGravityDeviceUpdateLocalTime(void);
extern const char *bplGetWirelessHydrometerName(void);
extern void bplSetGravity(float gravity);
extern void bplSetOriginalGravity(float og);

//#if EanbleParasiteTempControl
// glycol(parasiteTemperature)

extern float bplGetGlycolTemperature(void);
extern float bplGetGlycolSetTemp(void);
extern uint8_t bplGetGlycolState(void);
extern uint32_t bplGetGlycolElapsedTime(void);
//#endif
#define InvalidGravityDecreasedValue -10000.0
#define IsGravityDecreasedValueValid(g) (g)>-10000.0

extern float bplGetGravityDecreasedIn(int hour);

#if EnableHumidityControlSupport
extern uint8_t bplGetRoomHumidity(void);
extern uint8_t bplGetChamberHumidity(void);
extern uint8_t bplGetTargetHumidity(void);
extern uint8_t bplGetHumidityControlState(void);
#endif

typedef void (*HomekitStatusIndicationCB)(uint8_t);

extern uint8_t bplHomekitGetStatus(void);
extern void bplHomekitSetStatusCB(HomekitStatusIndicationCB cb);
bool bplHomekitClearPairing(void);
bool bplHomekitStartPairing(void);
void bplHomekitGetSetupUri(char ret[]);
const char* bplHomekitGetPairCode(void);
#if 0 //SIMULATOR 
struct tm {
int tm_sec; 
int tm_min; 
int tm_hour;
int tm_mday;
int tm_mon; 
int tm_year;
int tm_wday;
int tm_yday; 
int tm_isdst;
};
typedef uint32_t time_t; 
#define _TM_DEFINED

extern void localtime_r(const time_t timep,struct tm *result);
extern double atof(const char *str);

#endif

#ifdef __cplusplus
} //extern "C" {
#endif

#endif
