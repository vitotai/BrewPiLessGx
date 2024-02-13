
#include "ui.h"
#include "ui_helpers.h"


void cui_screenNetworkScan_screen_init(void);
void cui_event_screenNetworkScan( lv_event_t * e);
void onScreenNetworkScanLoaded(lv_event_t * e);

 lv_obj_t *cui_screenNetworkScan;
 lv_obj_t *cui_spinner;

void cui_screenNetworkList_screen_init(void);
void cui_event_screenNetworkList( lv_event_t * e);
void onScreenNetworkListLoadStart(lv_event_t * e);
void onScreenNetworkListUnloadStart(lv_event_t * e);

 lv_obj_t *cui_screenNetworkList;
 lv_obj_t *cui_tableNetworks;
 lv_obj_t *cui_btnNetworkScanBack;
void  cui_event_btnNetworkScanBack( lv_event_t * e);

void cui_screenNetworkScan_screen_scandone(void);
void cui_event_btnNetworkScanBack( lv_event_t * e);
void cui_event_table_event( lv_event_t * e);

void onTableItemSelected( uint16_t index);


void cui_screenNetworkScan_screen_init(void)
{
// create screen
cui_screenNetworkScan = lv_obj_create(NULL);
lv_obj_clear_flag( cui_screenNetworkScan, LV_OBJ_FLAG_SCROLLABLE );    /// Flags
lv_obj_set_style_bg_color(cui_screenNetworkScan, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_bg_opa(cui_screenNetworkScan, 255, LV_PART_MAIN| LV_STATE_DEFAULT);

/*Create a spinner*/
    lv_obj_t * cui_spinner = lv_spinner_create(cui_screenNetworkScan, 1000, 60);
    lv_obj_set_size(cui_spinner, 100, 100);
    lv_obj_center(cui_spinner);
/* call network scan and wait result*/

lv_obj_add_event_cb(cui_screenNetworkScan, cui_event_screenNetworkScan, LV_EVENT_ALL, NULL);

}

void cui_event_screenNetworkScan( lv_event_t * e) {
    lv_event_code_t event_code = lv_event_get_code(e);lv_obj_t * target = lv_event_get_target(e);
if ( event_code == LV_EVENT_SCREEN_LOADED) {
      onScreenNetworkScanLoaded( e );
}
}



void cui_screenNetworkList_screen_init(void){

// create screen
cui_screenNetworkList = lv_obj_create(NULL);
lv_obj_clear_flag( cui_screenNetworkList, LV_OBJ_FLAG_SCROLLABLE );    /// Flags
lv_obj_set_style_bg_color(cui_screenNetworkList, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_bg_opa(cui_screenNetworkList, 255, LV_PART_MAIN| LV_STATE_DEFAULT);


/* back button*/
cui_btnNetworkScanBack = lv_imgbtn_create(cui_screenNetworkList);
lv_imgbtn_set_src(cui_btnNetworkScanBack, LV_IMGBTN_STATE_RELEASED, NULL, &ui_img_backbutton_png, NULL);
lv_obj_set_width( cui_btnNetworkScanBack, 20);
lv_obj_set_height( cui_btnNetworkScanBack, 36);
lv_obj_set_x( cui_btnNetworkScanBack, 0 );
lv_obj_set_y( cui_btnNetworkScanBack, 0 );
lv_obj_set_align( cui_btnNetworkScanBack, LV_ALIGN_TOP_LEFT );
lv_obj_add_event_cb(cui_btnNetworkScanBack,cui_event_btnNetworkScanBack , LV_EVENT_ALL, NULL);
/* network list*/
     cui_tableNetworks = lv_table_create(cui_screenNetworkList);

    /*Set a smaller height to the table. It'll make it scrollable*/
    lv_obj_set_height( cui_tableNetworks, lv_pct(90));
    lv_obj_set_width( cui_tableNetworks, 220);
    lv_obj_set_x( cui_tableNetworks, 0 );
    lv_obj_set_y( cui_tableNetworks, 0 );
    lv_obj_set_align( cui_tableNetworks, LV_ALIGN_CENTER );
    
    lv_table_set_col_width(cui_tableNetworks, 0,160);
    lv_table_set_col_width(cui_tableNetworks, 1, 40);
    lv_table_set_col_cnt(cui_tableNetworks, 2);

    /*Don't make the cell pressed, we will draw something different in the event*/
    //lv_obj_remove_style(cui_tableNetworks, NULL, LV_PART_ITEMS | LV_STATE_PRESSED);

    /*Add an event callback to to apply some custom drawing*/
    //lv_obj_add_event_cb(cui_tableNetworks, nwlist_draw_event_cb, LV_EVENT_DRAW_PART_END, NULL);

//
    lv_obj_add_event_cb(cui_screenNetworkList, cui_event_screenNetworkList, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(cui_tableNetworks, cui_event_table_event,LV_EVENT_VALUE_CHANGED,NULL);
}


void cui_event_screenNetworkList( lv_event_t * e) {
    lv_event_code_t event_code = lv_event_get_code(e);lv_obj_t * target = lv_event_get_target(e);
if ( event_code == LV_EVENT_SCREEN_LOAD_START) {
      onScreenNetworkListLoadStart( e );
}else if(event_code == LV_EVENT_SCREEN_UNLOAD_START){
    onScreenNetworkListUnloadStart(e);
}
}



void cui_event_btnNetworkScanBack( lv_event_t * e) {
    lv_event_code_t event_code = lv_event_get_code(e);lv_obj_t * target = lv_event_get_target(e);
if ( event_code == LV_EVENT_CLICKED) {
      _ui_screen_change( &ui_screenSetting, LV_SCR_LOAD_ANIM_MOVE_RIGHT, 300, 0, &ui_screenSetting_screen_init);
}
}

void cui_event_table_event( lv_event_t * e) {
    lv_obj_t * obj = lv_event_get_target(e);
    uint16_t col;
    uint16_t row;
    lv_table_get_selected_cell(obj, &row, &col);
    onTableItemSelected(row);
}
