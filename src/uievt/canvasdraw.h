#ifndef CANVASDRAW_H
#define CANVASDRAW_H

#ifdef __cplusplus
extern "C" {
#endif

#include "lvgl.h"

void canvasDrawRectangle(lv_obj_t * canvas,lv_coord_t x, lv_coord_t y, lv_coord_t w, lv_coord_t h,lv_color_t color);
void canvasDrawLine(lv_obj_t * canvas, lv_coord_t x1, lv_coord_t y1, lv_coord_t x2, lv_coord_t y2,lv_color_t color);
void canvasDrawCircle(lv_obj_t * canvas,int xc, int yc, int r, lv_color_t color);
void canvasDrawCircleSafe(lv_obj_t * canvas,int xc, int yc, int r, lv_color_t color);
void canvasDrawCircleSafe_nv(lv_obj_t * canvas,int xc, int yc, int r, lv_color_t color);

#ifdef __cplusplus
} //extern "C" {
#endif

#endif