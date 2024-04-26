#ifndef DRIVER_IF_H
#define DRIVER_IF_H
#include "lvgl.h"
// All driver sould be implement the following functions
// Display
void display_drv_init(void);
void display_drv_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p);

void display_drv_sleep(void);
void display_drv_wakeup(void);
// Touch interface
void touch_drv_init(void);
void touch_drv_read(lv_indev_drv_t *indev_driver, lv_indev_data_t *data);

#endif