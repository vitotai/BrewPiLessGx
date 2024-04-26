#include <stdlib.h>
#include "networkscan.h"
#include "BrewPiInterface.h"
#include "inputscreen.h"

void passphraseInput(const char* text);
void fixedIPInput(const char* text);
void netmaskInput(const char* text);
void gatewayInput(const char* text);
void dnsIPInput(const char* text);

void voidCancel(void);
void netScanResultCb(WiFiListEntry* list,uint16_t count);
void openAutoIPConfirmDialog(void);
void connectNetwork(void);

static WiFiListEntry* nwlist;
static uint16_t nwcount;

static char nwpass[64];
static char ssid[64];
uint32_t fixedIp,netmask,gateway,nameserver;


static void event_network_selected_handler(lv_event_t * e){

    lv_event_code_t code = lv_event_get_code(e);
    if(code != LV_EVENT_CLICKED) {
        return;
    }

    unsigned long index =(unsigned long)lv_event_get_user_data(e);

    strcpy(ssid,nwlist[index].ssid);

    if(nwlist[index].enc){
        openInputScreen(LV_SCR_LOAD_ANIM_MOVE_LEFT,PASSPHRASE_TITLE,&passphraseInput,&voidCancel,InputTypeText,"");
    }else{
        nwpass[0]='\0';
        openAutoIPConfirmDialog();
    }
    // "free" nwlist?
}


static void requestWiFiScan(void* obj){
    bplRequestWiFiScan(&netScanResultCb);
}

void onScreenNetworkScanLoaded(lv_event_t * e){
    //removeTableIfCreated();
    lv_async_call(requestWiFiScan,NULL);
}


void connectNetwork(void){
    const char* pass=nwpass[0]? nwpass:NULL;

    bplConnectNetwork(ssid,pass,fixedIp,gateway,netmask,nameserver);
}


void netScanResultCb(WiFiListEntry* list,uint16_t count){
    if(count >0){
        nwcount=count;
        nwlist=list;    
        _ui_screen_change( &cui_screenNetworkList, LV_SCR_LOAD_ANIM_NONE, 100, 0, &cui_screenNetworkList_screen_init);
    }else{
        _ui_screen_change( &ui_screenSetting, LV_SCR_LOAD_ANIM_MOVE_RIGHT, 300, 0, &ui_screenSetting_screen_init);
    }
}
void onScreenNetworkListUnloadStart(lv_event_t * e){
   if(nwlist){
        free(nwlist);
        nwlist=NULL;
   }
}

void onScreenNetworkListLoadStart(lv_event_t * e){
    uint32_t i;
    lv_obj_t *btn;
    for(i = 0; i < nwcount; i++) {

        btn = lv_list_add_btn(cui_listNetworks, nwlist[i].enc? LV_SYMBOL_SETTINGS:LV_SYMBOL_WIFI,nwlist[i].ssid); 
        lv_obj_add_event_cb(btn, event_network_selected_handler, LV_EVENT_CLICKED,(void*) i);
    }
}



void voidCancel(void){
    // donotghin. Cancel is not allowed
}

void openAutoIPConfirmDialog(void){
    _ui_screen_change( &cui_screenQueryAutoIP, LV_SCR_LOAD_ANIM_MOVE_LEFT, 300, 0, &cui_screenQueryAutoIP_init);
}

void onAutoIPYes( lv_event_t * e){
    fixedIp=netmask=gateway=nameserver=0;
    connectNetwork();
    _ui_screen_change( &ui_screenSetting, LV_SCR_LOAD_ANIM_MOVE_RIGHT, 300, 0, &ui_screenSetting_screen_init);
}

void onAutoIPNo( lv_event_t * e){
    // fixed IP.
    // request to input IP, 
    openInputScreen(LV_SCR_LOAD_ANIM_MOVE_LEFT,IPADDRESS_TITLE,&fixedIPInput,&voidCancel,InputTypeNumber,IPADDRESS_DEFAULT);
}

void passphraseInput(const char* text){
    strcpy(nwpass,text);
    // save passphrase, and got to 
    openAutoIPConfirmDialog();
}

void fixedIPInput(const char* text){
    fixedIp = convertIp(text);

    setInputScreenTitle(NETMASK_TITLE);
    setInputScreenText(NETMASK_DEFAULT);
    setInputApplyCallback(&netmaskInput);
}
void netmaskInput(const char* text){
    netmask = convertIp(text);

    setInputScreenTitle(GATEWAY_TITLE);
    // do some calculation, maybe
    setInputScreenText(GATEWAY_DEFAULT);
    setInputApplyCallback(&gatewayInput);
}

void gatewayInput(const char* text){
    gateway = convertIp(text);

    setInputScreenTitle(DNS_TITLE);
    setInputScreenText(DNS_DEFAULT);
    setInputApplyCallback(&dnsIPInput);
}

void dnsIPInput(const char* text){
    // done. set, and back to setting screen
    nameserver = convertIp(text);

    connectNetwork();
    _ui_screen_change( &ui_screenSetting, LV_SCR_LOAD_ANIM_MOVE_RIGHT, 300, 0, &ui_screenSetting_screen_init);
}
