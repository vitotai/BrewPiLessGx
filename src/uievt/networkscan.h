#ifndef NETWORKSCAN_H
#define NETWORKSCAN_H

#ifdef __cplusplus
extern "C" {
#endif

    #include "lvgl.h"

#include "ui_helpers.h"
#include "ui_events.h"

#define PASSPHRASE_TITLE "Passphrase"

#define IPADDRESS_TITLE "IP Address"
#define IPADDRESS_DEFAULT "192.168.0."

#define NETMASK_TITLE "Netmask"
#define NETMASK_DEFAULT "255.255.0.0"

#define GATEWAY_TITLE "Gateway"
#define GATEWAY_DEFAULT "192.168.0.1"

#define DNS_TITLE "Gateway"
#define DNS_DEFAULT "8.8.8.8"


#define QUERY_FIXED_IP "Automatic IP?"
#define DIALOG_YES "YES"
#define DIALOG_NO "NO"

/* scanning screen*/
void cui_screenNetworkScan_screen_init(void);
void cui_event_screenNetworkScan( lv_event_t * e);
void onScreenNetworkScanLoaded(lv_event_t * e);

extern lv_obj_t *cui_screenNetworkScan;
extern lv_obj_t *cui_spinner;

/* list screen*/
void cui_screenNetworkList_screen_init(void);
void cui_event_screenNetworkList( lv_event_t * e);
void onScreenNetworkListLoadStart(lv_event_t * e);
void onScreenNetworkListUnloadStart(lv_event_t * e);

extern lv_obj_t *cui_screenNetworkList;
extern lv_obj_t *cui_tableNetworks;
extern lv_obj_t *cui_btnNetworkScanBack;
void  cui_event_btnNetworkScanBack( lv_event_t * e);

void cui_screenNetworkScan_screen_scandone(void);
void cui_event_btnNetworkScanBack( lv_event_t * e);
void cui_event_table_event( lv_event_t * e);

void onTableItemSelected( uint16_t index);

/* fixed IP screen */
void cui_screenQueryAutoIP_init();
extern lv_obj_t *cui_screenQueryAutoIP;
extern lv_obj_t *cui_btnQueryAutoIPYes;
extern lv_obj_t *cui_btnQueryAutoIPNo;

void onAutoIPYes( lv_event_t * e);
void onAutoIPNo( lv_event_t * e);
#ifdef __cplusplus
} //extern "C" {
#endif

#endif