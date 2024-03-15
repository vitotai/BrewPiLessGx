#include "Arduino.h"
#include "math.h"
#include "lvgl.h"
#include "tft_functions.h"
#include "ledstatus.h"
#include "TempControl.h"
#if EnableColorLED
const lv_color32_t StateColor[]={
	{.full = 0x002200}, // idle
	{.full = 0x220000}, // off
	{.full = 0xAAAAAA}, // open
	{.full = 0xFFA500}, // heating
	{.full = 0x000088}, // cooling
	{.full = 0x100010}, // wait to cool
	{.full = 0x221100}, // wait to heat
	{.full = 0x555555}, // wait for peak
	{.full = 0x000055}, // cool min time
	{.full = 0x553300}, // heat min time
	{.full = 0xFF0000} // invalid
};


 LedStatusClass LedStatus;

 LedStatusClass::LedStatusClass(void){
    _state=NUM_STATES;
    set_led_color(lv_color32_t({.ch = {.blue=0,.green = 0x00,.red=0}}));
 }


void LedStatusClass::setState(uint8_t state){
    if(_state == state) return;
   _state = state;
   set_led_color(StateColor[state]);
}


 void LedStatusClass::loop(void){
/*    if(_state == IDLE){
        float val = (exp(sin(millis()/2000.0 * PI)) - 0.368) * 108.0;
        lv_color32_t target=StateColor[_state];
        lv_color32_t color;
        color.ch.blue = target.ch.blue * val;
        color.ch.red = target.ch.red * val;
        color.ch.green = target.ch.green * val;
        set_led_color(color);
    }
    */
}
#endif