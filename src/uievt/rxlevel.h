#ifndef RXLEVEL_H
#define RXLEVEL_H

#ifdef __cplusplus
extern "C" {
#endif

#include "lvgl.h"

typedef struct _rxlevel_widget{
lv_obj_t *canvas;
lv_color_t *buffer;
uint16_t width;
uint16_t height;
uint16_t level;
bool  disabled;
}rxlevel_widget;


extern rxlevel_widget *lv_rxlevel_create(lv_obj_t *parent,uint16_t width,uint16_t height);

extern void lv_rxlevel_setDisabled(rxlevel_widget* widget,bool disabled);

extern void lv_rxlevel_setLevel(rxlevel_widget* widget,uint8_t level);
extern void lv_rxlevel_setBgColor(rxlevel_widget* widget,lv_color_t color);
extern void lv_rxlevel_setColor(rxlevel_widget* widget,lv_color_t color);
extern void lv_rxlevel_setSubColor(rxlevel_widget* widget,lv_color_t color);

extern lv_obj_t *lv_rxlevel_getCanvas(rxlevel_widget* widget);

#ifdef __cplusplus
}  //extern "C" {
#endif

#endif