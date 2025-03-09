#include <lvgl.h>
#include <ui.h>
#include <Arduino.h>
#include "lv_port_fs_littlefs.h"
#include "wakebutton.h"
#include "lv_drv_conf.h"
#include "driver_if.h"

extern void bpl_setup(void);
extern void bpl_loop(void);

#define DrawBufferSize  LV_DRAW_BUFFER_SIZE * (LV_COLOR_DEPTH / 8)

static uint32_t my_tick(void)
{
    return millis();
}


void setup(){
    bpl_setup();
    lv_init();
    lv_tick_set_cb(my_tick);

    // LVGL display driver stup
    display_drv_init();
    static uint8_t *disp_draw_buf = (uint8_t *)heap_caps_malloc(DrawBufferSize, MALLOC_CAP_INTERNAL | MALLOC_CAP_8BIT);
    if (!disp_draw_buf){
      Serial.println("LVGL disp_draw_buf allocate failed!");
    }else{

      static auto *lvDisplay = lv_display_create(CONFIG_LVGL_DISPLAY_WIDTH, CONFIG_LVGL_DISPLAY_HEIGHT);
      lv_display_set_color_format(lvDisplay, LV_COLOR_FORMAT_RGB565);
      lv_display_set_flush_cb(lvDisplay,display_drv_flush);
      lv_display_set_buffers(lvDisplay, disp_draw_buf, nullptr, DrawBufferSize, LV_DISPLAY_RENDER_MODE_PARTIAL);
    }

    // LVGL touch driver
    #if TOUCH_INPUT_ENABLE    
    touch_drv_init();
    /* Initialize the (dummy) input device driver */
    lv_indev_t * indev = lv_indev_create();
    lv_indev_set_type(indev, LV_INDEV_TYPE_POINTER); 
    lv_indev_set_read_cb(indev, touch_drv_read); 

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
