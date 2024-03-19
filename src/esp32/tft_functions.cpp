#include <lvgl.h>
#include <ui.h>
#include <Arduino.h>
#include <Arduino_GFX_Library.h>
extern Arduino_GFX *gfx;

#include "tft_functions.h"

static bool isTftSleeping=false;

bool is_tft_sleeping(){ return isTftSleeping;}

void tft_sleep(void){
    gfx->displayOff();
    isTftSleeping=true;
}

void tft_wakeup(void){
    gfx->displayOn();
    delay(10);
    isTftSleeping=false;
}
