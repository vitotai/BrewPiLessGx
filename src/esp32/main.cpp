#include <lvgl.h>
#include <ui.h>
#include <Arduino.h>
#include "lv_drv_conf.h"
#include "lv_port_fs_littlefs.h"
#include "wakebutton.h"

extern void display_setup();
extern void touch_setup();

extern void bpl_setup(void);
extern void bpl_loop(void);
extern void driver_setup();

void setup()
{
    bpl_setup();
    display_setup();
    #if TOUCH_INPUT_ENABLE
    touch_setup();
    #endif
    // after "bpl_setup()",before ui_init();
    lv_port_littlefs_init();

    ui_init();
    #if WAKEUP_BUTTON_ENABLED
    wakebutton_init();
    #endif
}

void loop()
{

    lv_timer_handler(); /* let the GUI do its work */
    bpl_loop();
    delay(5);
    #if WAKEUP_BUTTON_ENABLED
    if(wakebutton_pressed()){
        screenWakeup();
    }
    #endif
}
