#include <lvgl.h>
#include <ui.h>
#include <Arduino.h>
#include "lv_port_fs_littlefs.h"
#include "wakebutton.h"
#include "lv_drv_conf.h"
#include "driver_if.h"

extern void bpl_setup(void);
extern void bpl_loop(void);

void setup(){
    bpl_setup();
    lv_init();

    // LVGL display driver stup
    display_drv_init();
    static lv_disp_draw_buf_t draw_buf;

    static lv_color_t *disp_draw_buf = (lv_color_t *)heap_caps_malloc(sizeof(lv_color_t) * LV_DRAW_BUFFER_SIZE, MALLOC_CAP_INTERNAL | MALLOC_CAP_8BIT);
    if (!disp_draw_buf){
      Serial.println("LVGL disp_draw_buf allocate failed!");
    }else{
      lv_disp_draw_buf_init(&draw_buf, disp_draw_buf, NULL, LV_DRAW_BUFFER_SIZE);
      /* Initialize the display */
      static lv_disp_drv_t disp_drv;
      lv_disp_drv_init(&disp_drv);
      /* Change the following line to your display resolution */
      disp_drv.hor_res = CONFIG_LVGL_DISPLAY_WIDTH;
      disp_drv.ver_res = CONFIG_LVGL_DISPLAY_HEIGHT;
      disp_drv.flush_cb = display_drv_flush;
      disp_drv.draw_buf = &draw_buf;
      lv_disp_drv_register(&disp_drv);      
      lv_obj_clean(lv_scr_act());
    }

    // LVGL touch driver
    #if TOUCH_INPUT_ENABLE    
    touch_drv_init();
    /* Initialize the (dummy) input device driver */
    static lv_indev_drv_t indev_drv;
    lv_indev_drv_init(&indev_drv);
    indev_drv.type = LV_INDEV_TYPE_POINTER;
    indev_drv.read_cb = touch_drv_read;
    lv_indev_drv_register(&indev_drv);
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
