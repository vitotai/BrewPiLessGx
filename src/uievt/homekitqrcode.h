#ifndef HOMEKIT_QRCODE_H
#define HOMEKIT_QRCODE_H
#ifdef __cplusplus
extern "C" {
#endif

#include "lvgl.h"

void show_homekit_qrcode(lv_obj_t * retscreen);
void dismiss_homekit_qrcode(void);

#ifdef __cplusplus
} //extern "C" {
#endif

#endif
