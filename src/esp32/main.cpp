#include <lvgl.h>
#include <ui.h>
#include <Arduino.h>

#if 1 //ESP32_17320S019N

extern void display_setup();
extern void touch_setup();

#else
#include <esp32_smartdisplay.h>
#endif
//#include <lvgl_helpers.h>
//#include "lv_drv_conf.h"
#include "lv_port_fs_littlefs.h"


extern void bpl_setup(void);
extern void bpl_loop(void);
extern void driver_setup();

void setup()
{
    bpl_setup();
//    driver_setup();
    #if 1 //ESP32_17320S019N    
    display_setup();
    touch_setup();
    #else
    smartdisplay_init();
    smartdisplay_set_led_color( lv_color32_t({.ch = {0xFF,0xFF,0xFF,0xFF}}));
    #endif
    // after "bpl_setup()",before ui_init();
    lv_port_littlefs_init();

    ui_init();

}

void loop()
{

    lv_timer_handler(); /* let the GUI do its work */
    bpl_loop();
    delay(5);
}
