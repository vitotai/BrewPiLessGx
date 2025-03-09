#include "Arduino.h"
#include "math.h"
#include "ledstatus.h"
#include "TempControl.h"
#if EnableColorLED

const lv_color_t StateColor[]={
	{.blue=0x00, .green=0x22, .red=0x00}, // idle
	{.blue=0x00, .green=0x00, .red=0x00}, // off
	{.blue=0xAA, .green=0xAA, .red=0xAA}, // open
	{.blue=0x00, .green=0xA5, .red=0xFF}, // heating
	{.blue=0x88, .green=0x00, .red=0x00}, // cooling
	{.blue=0x10, .green=0x00, .red=0x10}, // wait to cool
	{.blue=0x00, .green=0x11, .red=0x22}, // wait to heat
	{.blue=0x55, .green=0x55, .red=0x55}, // wait for peak
	{.blue=0x55, .green=0x00, .red=0x00}, // cool min time
	{.blue=0x00, .green=0x33, .red=0x55}, // heat min time
	{.blue=0x00, .green=0x00, .red=0xFF} // invalid
};


 LedStatusClass LedStatus;

 LedStatusClass::LedStatusClass(void){
    _state=NUM_STATES;
    pinMode(LED_PIN_R, OUTPUT);
    digitalWrite(LED_PIN_R, true);
    ledcSetup(LED_PWM_CHANNEL_R, LED_PWM_FREQ, LED_PWM_BITS);
    ledcAttachPin(LED_PIN_R, LED_PWM_CHANNEL_R);

    pinMode(LED_PIN_G, OUTPUT);
    digitalWrite(LED_PIN_G, true);
    ledcSetup(LED_PWM_CHANNEL_G, LED_PWM_FREQ, LED_PWM_BITS);
    ledcAttachPin(LED_PIN_G, LED_PWM_CHANNEL_G);

    pinMode(LED_PIN_B, OUTPUT);
    digitalWrite(LED_PIN_B, true);
    ledcSetup(LED_PWM_CHANNEL_B, LED_PWM_FREQ, LED_PWM_BITS);
    ledcAttachPin(LED_PIN_B, LED_PWM_CHANNEL_B);

    _set_led_color({.blue=0,.green = 0x00,.red=0});
 }


void LedStatusClass::setState(uint8_t state){
    if(_state == state) return;
   _state = state;
   _set_led_color(StateColor[state]);
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

void LedStatusClass::_set_led_color(lv_color_t rgb)
{
  ledcWrite(LED_PWM_CHANNEL_R, LED_PWM_MAX - rgb.red);
  ledcWrite(LED_PWM_CHANNEL_G, LED_PWM_MAX - rgb.green);
  ledcWrite(LED_PWM_CHANNEL_B, LED_PWM_MAX - rgb.blue);
}

#endif
