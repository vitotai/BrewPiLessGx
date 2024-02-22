#include <lvgl.h>
#include <ui.h>
#include <Arduino.h>
#if ESP32_17320S019N
#include <Arduino_GFX_Library.h>
extern Arduino_GFX *gfx;
#else
#include <esp32_smartdisplay.h>
#endif

#include "tft_functions.h"

static bool isTftSleeping=false;

bool is_tft_sleeping(){ return isTftSleeping;}

void tft_sleep(void){
#if ESP32_17320S019N
    gfx->displayOff();
#else
    smartdisplay_tft_sleep();
#endif
    isTftSleeping=true;
    Serial.print("*****Sleep****\n");
}

void tft_wakeup(void){
#if ESP32_17320S019N
    gfx->displayOn();
#else
    smartdisplay_tft_wake();
#endif
    delay(10);
    isTftSleeping=false;
    Serial.print("***** wakeup ****\n");
}
void set_led_color_rgb(uint8_t r,uint8_t g,uint8_t b){
#if ESP32_17320S019N
#else
    lv_color32_t color;
    color.ch.red=r;
    color.ch.green=g;
    color.ch.blue=b;
    smartdisplay_set_led_color(color);
#endif
}

void set_led_color(lv_color32_t color){
#if ESP32_17320S019N
#else    
    smartdisplay_set_led_color(color);
#endif
}