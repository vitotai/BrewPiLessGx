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

static void requestWiFiScan(void* unused){
    bplRequestWiFiScan(&netScanResultCb);
}

void onScreenNetworkScanLoaded(lv_event_t * e){
    //removeTableIfCreated();
    lv_async_call(requestWiFiScan,NULL);
}
static WiFiListEntry* nwlist;
static uint16_t nwcount;

static char nwpass[64];
static char ssid[64];
uint32_t fixedIp,netmask,gateway,nameserver;



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

    lv_table_set_row_cnt(cui_tableNetworks, nwcount); /*Not required but avoids a lot of memory reallocation lv_table_set_set_value*/
    uint32_t i;
    for(i = 0; i < nwcount; i++) {
        lv_table_set_cell_value(cui_tableNetworks, i, 0, nwlist[i].ssid);
        lv_table_set_cell_value(cui_tableNetworks, i, 1, nwlist[i].enc? "S":"");
    }
}

void onTableItemSelected(uint16_t index){

    strcpy(ssid,nwlist[index].ssid);

    if(nwlist[index].enc){
        openInputScreen(LV_SCR_LOAD_ANIM_MOVE_LEFT,PASSPHRASE_TITLE,&passphraseInput,&voidCancel,InputTypeText,"");
    }else{
        nwpass[0]='\0';
        openAutoIPConfirmDialog();
    }
    // "free" nwlist?
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
