#include "networkscan.h"



/* fixed IP screen */
void cui_screenQueryAutoIP_init();
lv_obj_t *cui_screenQueryAutoIP;
lv_obj_t *cui_btnQueryAutoIPYes;
lv_obj_t *cui_btnQueryAutoIPNo;

void onAutoIPYes( lv_event_t * e);
void onAutoIPNo( lv_event_t * e);


void cui_screenQueryAutoIP_init(){

    // create screen
    cui_screenQueryAutoIP = lv_obj_create(NULL);
    // create Label

    lv_obj_t * label;
    // query stirng
    label = lv_label_create(cui_screenQueryAutoIP);
    lv_label_set_text(label,QUERY_FIXED_IP);
    lv_obj_align(label,LV_ALIGN_CENTER,0,-36);

    // button 1
    cui_btnQueryAutoIPYes = lv_btn_create(cui_screenQueryAutoIP);
    lv_obj_align(cui_btnQueryAutoIPYes, LV_ALIGN_CENTER, 80, 10);
    lv_obj_add_event_cb(cui_btnQueryAutoIPYes, onAutoIPYes, LV_EVENT_CLICKED, NULL);

    label = lv_label_create(cui_btnQueryAutoIPYes);
    lv_label_set_text(label, DIALOG_YES);
    lv_obj_center(label);


    // button 2
    cui_btnQueryAutoIPNo = lv_btn_create(cui_screenQueryAutoIP);
    lv_obj_align(cui_btnQueryAutoIPNo, LV_ALIGN_CENTER, -80, 10);
    lv_obj_add_event_cb(cui_btnQueryAutoIPNo, onAutoIPNo, LV_EVENT_CLICKED, NULL);

    label = lv_label_create(cui_btnQueryAutoIPNo);
    lv_label_set_text(label, DIALOG_NO);
    lv_obj_center(label);
}
