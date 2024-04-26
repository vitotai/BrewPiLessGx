#ifndef LEDSTATUS_H
#define LEDSTATUS_H
#include "lvgl.h"


// Build in RGB LED
#define LED_PIN_R 4
#define LED_PIN_G 16
#define LED_PIN_B 17
// PWM channels for RGB
#define LED_PWM_FREQ 5000
#define LED_PWM_CHANNEL_R 13
#define LED_PWM_CHANNEL_G 14
#define LED_PWM_CHANNEL_B 15
#define LED_PWM_BITS 8
#define LED_PWM_MAX ((1 << LED_PWM_BITS) - 1)


class LedStatusClass{
protected:
    uint8_t _state;
    void _set_led_color(lv_color32_t rgb);

public:
    LedStatusClass(void);
    void setState(uint8_t state);
    void loop(void);
};

extern LedStatusClass LedStatus;

#endif