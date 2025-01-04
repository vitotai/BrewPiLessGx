#include <stdlib.h>
#include <string.h>

#include "lvgl.h"
#include "src/extra/libs/qrcode/lv_qrcode.h"
#include "BrewPiInterface.h"
#include "homekitqrcode.h"

#if HOMEKIT_UI
void homekit_setup_uri(char ret[],unsigned int category,const char* setup_code,const char* setup_id,int ble_pair, int ip_pair,int nfc_pair);
long long con_setup_code(const char* code);
void pair_screen_init(void);

static lv_obj_t *pair_screen;
static lv_obj_t *screen_to_return;

static void homekitStatusCb(uint8_t status){
	if(status >2) status =2;

	if(status != 1){
		// pairing timeout or something else
        dismiss_homekit_qrcode();
	}
}

void show_homekit_qrcode(lv_obj_t * retscreen){
    screen_to_return = retscreen;
    pair_screen_init();
    lv_scr_load_anim(pair_screen, LV_SCR_LOAD_ANIM_MOVE_BOTTOM, 300, 0, false);
    
    bplHomekitSetStatusCB(homekitStatusCb);
}

void dismiss_homekit_qrcode(void){
    bplHomekitSetStatusCB(NULL);
   lv_scr_load_anim(screen_to_return, LV_SCR_LOAD_ANIM_MOVE_TOP, 300, 0, true); // auto delete
}

static void ui_event_btn_close( lv_event_t * e) {
    lv_event_code_t event_code = lv_event_get_code(e);
	if ( event_code == LV_EVENT_CLICKED) {
        dismiss_homekit_qrcode();
    }
}

void pair_screen_init(void){
    pair_screen = lv_obj_create(NULL);
    lv_obj_clear_flag( pair_screen, LV_OBJ_FLAG_SCROLLABLE );    /// Flags
    lv_obj_set_style_bg_color(pair_screen,lv_color_black(), LV_PART_MAIN| LV_STATE_DEFAULT);

	char uri[22];
	//homekit_setup_uri(uri,5,"466-37-726","HSPN",0,1,0);
    bplHomekitGetSetupUri(uri);

    lv_obj_t * qr = lv_qrcode_create(pair_screen, 150, lv_color_black(), lv_color_white());

    /*Set data*/
    lv_qrcode_update(qr, uri, strlen(uri));
    lv_obj_center(qr);

    /*Add a border with bg_color*/
    lv_obj_set_style_border_color(qr, lv_color_white(), 0);
    lv_obj_set_style_border_width(qr, 5, 0);
    //
    lv_obj_t* label=lv_label_create(pair_screen);
    lv_label_set_text_fmt(label, "Pair Code:%s",bplHomekitGetPairCode());
    lv_obj_set_align( label, LV_ALIGN_TOP_MID );
    lv_obj_set_y( label, 4 );

    label=lv_label_create(pair_screen);
    lv_label_set_text(label, "use Home App to pair");
    lv_obj_set_align( label, LV_ALIGN_BOTTOM_MID );
    lv_obj_set_y( label, -4 );

    lv_obj_t* btn=lv_btn_create(pair_screen);
    lv_obj_set_align( btn, LV_ALIGN_TOP_RIGHT );

    label=lv_label_create(btn);
    lv_label_set_text(label, LV_SYMBOL_CLOSE);
    
    lv_obj_add_event_cb(btn,ui_event_btn_close,LV_EVENT_CLICKED,NULL);
}

#endif
