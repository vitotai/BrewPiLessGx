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

extern	float BrewPiGetBeerTemp();
extern	float BrewPiGetBeerSet();
extern	float BrewPiGetFridgeTemp();
extern	float BrewPiGetFridgeSet();
extern	float BrewPiGetRoomTemp();
extern	char BrewPiGetUnit();
extern	float BrewPiGetMinSetTemp();
extern	float BrewPiGetMaxSetTemp();
extern	uint8_t BrewPiGetState();
extern	uint8_t BrewPiGetMode();
extern uint32_t BrewPiGetStatusTime();

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


extern uint8_t bplGetWiFiMode();
extern void bplSetWiFiMode(uint8_t newMode);
extern int     bplGetWiFiRssi();
extern bool    bplIsWiFiConnected();
extern void bplConnectNetwork(const char* ssid,const char* passphase,uint32_t ip,uint32_t gw,uint32_t nm,uint32_t dns);

extern const char* bplGetSsid();
extern uint32_t bplGetIpAddress();
extern const char* bplHostname();
extern const char* bplUsername();
extern const char* bplPassword();
extern void bplRequestWiFiScan(void (*result_cb)(WiFiListEntry* list,uint16_t count) );

extern float bplGetPressure();

extern bool bplIsUsingPlato();
extern float bplGravityDeviceTemperature();
extern float bplGravityDeviceTilt();
extern float bplGravityDeivceBattery();
extern char bplGravityDeviceBatteryUint();
extern int   bplGravityDeviceRssi();
extern float bplGetGravity();
extern float bplGetOriginalGravity();
extern uint32_t bplGetGravityDeviceUpdateLocalTime();
extern const char *bplGetWirelessHydrometerName();
extern void bplSetGravity(float gravity);
extern void bplSetOriginalGravity(float og);

//#if EanbleParasiteTempControl
// glycol(parasiteTemperature)

extern float bplGetGlycolTemperature();
extern float bplGetGlycolSetTemp();
extern uint8_t bplGetGlycolState();
extern uint32_t bplGetGlycolElapsedTime();
//#endif
#define InvalidGravityDecreasedValue -10000.0
#define IsGravityDecreasedValueValid(g) (g)>-10000.0

extern float bplGetGravityDecreasedIn(int hour);

#if EnableHumidityControlSupport
extern uint8_t bplGetRoomHumidity();
extern uint8_t bplGetChamberHumidity();
extern uint8_t bplGetTargetHumidity();
extern uint8_t bplGetHumidityControlState();
#endif
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
