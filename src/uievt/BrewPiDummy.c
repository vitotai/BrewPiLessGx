
#if SIMULATOR

#include "lvgl.h"
#include "BrewPiInterface.h"
//#define SkinFileName "A:/skin-default324.json"
#define SkinFileName "A:/simpleskin.json"

static uint8_t brewpi_mode=0;
static float brewpi_beerset=20;

static float brewpi_fridgeset=35;


uint8_t getSkin(void){
    return 1;
}

float BrewPiGetBeerTemp(){ return 122.2;}
float BrewPiGetBeerSet(){ return brewpi_beerset;}
float BrewPiGetFridgeTemp(){return 101.1;}
float BrewPiGetFridgeSet(){ return brewpi_fridgeset;}
float BrewPiGetRoomTemp(){ return -255.0;}

char BrewPiGetUnit(){ return 'C';}

int BrewPiGetMinSetTemp(){ return 1.0; }
int BrewPiGetMaxSetTemp(){ return 43.9;}

uint8_t BrewPiGetState(){ return 4;}

uint8_t BrewPiGetMode(){ return brewpi_mode;}

uint32_t BrewPiGetStatusTime(){ return 321;}

void bplConnectNetwork(const char* ssid,const char* passphase,uint32_t ip,uint32_t gw,uint32_t nm,uint32_t dns){
    
}


bool BrewPiAmbientSensorConnected(void)
{
	return false;
}


void BrewPiSetMode(uint8_t mode){
	brewpi_mode=mode;
}

void BrewPiSetBeer(int temp){
	brewpi_beerset=(float)temp;
}

void BrewPiSetFridge(int temp){
	brewpi_fridgeset=(float)temp;
}



// BPL
static uint8_t _wifiMode=WIFI_OFF;

uint8_t bplGetWiFiMode(){
	return _wifiMode;
}
void bplSetWiFiMode(uint8_t newMode){
    _wifiMode = newMode;
}
int     bplGetWiFiRssi(){
	return -80;
}

bool    bplIsWiFiConnected(){
	return _wifiMode == WIFI_STA || _wifiMode == WIFI_AP_STA;
}

const char* bplGetSsid(){
	return "VH";
}

uint32_t bplGetIpAddress(){
	return (192<<24) | (168<<16) | (0 <<8) | 112;
}

const char* bplHostname(){
	return "brewpilessGx";
}

const char* bplUsername(){
	return "vito";
}

const char* bplPassword(){
	return "mypass";
}

bool bplIsUsingPlato(){
	return true;
}

float bplGravityDeviceTemperature(){
	return 20.9;
}

float bplGravityDeviceTilt(){
	return 25.32;
}

float bplGravityDeivceBattery(){
	return 3.89;
}

static float _gravity=6.1;
static float _og=19.8;

float bplGetGravity(){
	return _gravity;
}

float bplGetOriginalGravity(){
	return _og;
}

void bplSetGravity(float gravity){
	_gravity = gravity;
}

void bplSetOriginalGravity(float og){
	_og = og;
}

int   bplGravityDeviceRssi(){
	return -69;
}

uint32_t bplGetGravityDeviceUpdateLocalTime(){
	return 1697489423;
}

float bplGetPressure(){
	return 11.8;
}


#define isspace(a) ((a)==' ' || (a) == '\t')
#define isdigit(a) ((a)>='0' && (a)<='9')

double atof(const char *str) {
    double result = 0.0;
    double sign = 1.0;
    double power = 1.0;

    // Skip leading white space
    while (isspace(*str)) {
        str++;
    }

    // Handle sign (if any)
    if (*str == '-') {
        sign = -1.0;
        str++;
    } else if (*str == '+') {
        str++;
    }

    // Process the integer part
    while (isdigit(*str)) {
        result = result * 10.0 + (*str - '0');
        str++;
    }

    // Process the fractional part
    if (*str == '.') {
        str++;
        while (isdigit(*str)) {
            result = result * 10.0 + (*str - '0');
            power *= 10.0;
            str++;
        }
    }

    result = result / power;

    // Apply the sign
    result *= sign;

    return result;
}

void my_timer(lv_timer_t * timer);

static void (*scan_result_cb)(WiFiListEntry* list,uint16_t count);

void bplRequestWiFiScan(void (*result_cb)(WiFiListEntry* list,uint16_t count) ){
    scan_result_cb=result_cb;
    // faked by starting a timer
    lv_timer_t * timer = lv_timer_create(my_timer, 3000,  NULL);
    lv_timer_set_repeat_count(timer, 1);
}

static WiFiListEntry testList[]={
{
    "VH",
    -60,
    true
},
{
    "VH2",
    -80,
    true
},
{
    "VH3",
    -100,
    false
},
{
    "VH4",
    -60,
    true
},
{
    "VH5",
    -60,
    true
},
{
    "VH6",
    -60,
    true
},
{
    "VH7",
    -60,
    true
}

};

void my_timer(lv_timer_t * timer){
    scan_result_cb(testList,sizeof(testList)/sizeof(WiFiListEntry));
}



uint32_t convertIp(const char* ip){
    unsigned long result = 0;
    unsigned long byteValue = 0;
    int shift = 24; // Start with the most significant byte.

    for (int i = 0; ip[i] != '\0'; i++) {
        if (ip[i] >= '0' && ip[i] <= '9') {
            byteValue = byteValue * 10 + (ip[i] - '0');
        } else if (ip[i] == '.') {
            if (byteValue > 255) {
                return 0;
            }
            result += (byteValue << shift);
            byteValue = 0;
            shift -= 8;
        } else {
            return 0;
        }
    }

    if (byteValue > 255) {
        return 0;
    }

    result += byteValue; // Add the last byte.

    return result;
}
#endif
