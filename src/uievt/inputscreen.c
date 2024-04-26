#include "lvgl.h"
#include "ui.h"
#include "ui_helpers.h"
#include "inputscreen.h"


void finishedInput(void);
void cancelInput(void);

void setInputScreenTitle(const char* title);
void setInputScreenText(const char* text);
void setInputApplyCallback(void (*input_applied)(const char*));

void (*_input_applied_cb)(const char*);
void (*_input_cancel_cb)(void);

void openInputScreen(lv_scr_load_anim_t anitype,const char* title,void (*input_applied)(const char*),void (*input_cancel)(void),InputType type,const char *text){

    _input_applied_cb = input_applied;
    _input_cancel_cb = input_cancel;
    
    lv_label_set_text(ui_lbInputLabel,title);

    if(type == InputTypeNumber){
        lv_keyboard_set_mode(ui_kbInput,LV_KEYBOARD_MODE_NUMBER);
    }else  if(type == InputTypeText){
        lv_keyboard_set_mode(ui_kbInput,LV_KEYBOARD_MODE_TEXT_LOWER);
    }
    lv_textarea_set_text(ui_taInput,text);
    
    _ui_screen_change( &ui_screenInput, anitype, 300, 0, &ui_screenInput_screen_init);
}

void finishedInput(void){
    _input_applied_cb(lv_textarea_get_text(ui_taInput));
}   

void cancelInput(void){
    _input_cancel_cb();
}

void setInputScreenTitle(const char* title){
    lv_label_set_text(ui_lbInputLabel,title);
}

void setInputScreenText(const char* text){
    lv_textarea_set_text(ui_taInput,text);
}

void setInputApplyCallback(void (*input_applied)(const char*)){
    _input_applied_cb = input_applied;
}
