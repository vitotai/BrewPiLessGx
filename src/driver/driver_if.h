#ifndef DRIVER_IF_H
#define DRIVER_IF_H
#include "lvgl.h"
// All driver sould be implement the following functions
// Display
#define MinimumBrightness 100


#ifdef __cplusplus
extern "C" {
#endif

void display_drv_init(void);
void display_drv_flush(lv_display_t * display, const lv_area_t * area, uint8_t * px_map);

void display_drv_sleep(void);
void display_drv_wakeup(void);
void display_drv_set_brightness(uint8_t brightness);

// Touch interface
void touch_drv_init(void);
void touch_drv_read(lv_indev_t * indev, lv_indev_data_t*data);

#ifdef __cplusplus
}
#endif

#endif
