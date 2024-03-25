#include <lvgl.h>
#include <ui.h>
#include <Arduino.h>
#include "lv_drv_conf.h"

#ifdef LGFX_DRIVER
#define LGFX_USE_V1
#ifdef ESP32_2432S032C
#include "LGFX_custom/LGFX_ESP32_2432S032C.h"
#endif // ESP32_2432S032C

#ifdef ESP32_2432S028R
#include "LGFX_custom/LGFX_ESP32_2432S028R.h"
#endif // ESP32_2432S028R

LGFX gfx;


#ifdef __cplusplus
extern "C" {
#endif
void display_drv_sleep(void);
void display_drv_wakeup(void);
#ifdef __cplusplus
}
#endif

void display_drv_sleep(void){
   gfx.sleep();
}
void display_drv_wakeup(void){
   gfx.wakeup();
}

/* Display flushing */
void display_drv_flush( lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p )
{
    uint32_t w = (area->x2 - area->x1 + 1);
    uint32_t h = (area->y2 - area->y1 + 1);

    gfx.startWrite();
    gfx.setAddrWindow(area->x1, area->y1, w, h);
    gfx.writePixelsDMA((lgfx::rgb565_t *)&color_p->full, w * h);
    gfx.endWrite();

    lv_disp_flush_ready(disp);
}

/*Read the touchpad*/
void touch_drv_read(lv_indev_drv_t * indev_driver, lv_indev_data_t * data )
{
    uint16_t touchX, touchY;

    data->state = LV_INDEV_STATE_REL;

    if( gfx.getTouch( &touchX, &touchY ) )
    {
        data->state = LV_INDEV_STATE_PR;

        /*Set the coordinates*/
        data->point.x = touchX;
        data->point.y = touchY;
    }
}

void display_drv_init()
{
    gfx.init();
    gfx.initDMA();
#ifdef SCREEN_ROTATION    
    gfx.setRotation(SCREEN_ROTATION);
#endif
}
void touch_drv_init(){}

#endif //#ifdef LGFX_DRIVER