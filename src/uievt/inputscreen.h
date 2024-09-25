#ifndef INPUTSCREEN_H
#define INPUTSCREEN_H

#ifdef __cplusplus
extern "C" {
#endif

#include "lvgl.h"

typedef enum _InputType{
    InputTypeNumber,
    InputTypeText
}InputType;

void openInputScreen(lv_scr_load_anim_t anitype,const char* title,void (*input_applied)(const char*),void (*input_cancel)(void),InputType type,const char *text,bool auto_del);
void finishedInput(void);
void cancelInput(void);

void setInputScreenTitle(const char* title);
void setInputScreenText(const char* text);
void setInputApplyCallback(void (*input_applied)(const char*));

#ifdef __cplusplus
} //extern "C" {
#endif

#endif
