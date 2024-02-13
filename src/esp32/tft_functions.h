#ifndef TFTFUNCTIONS_H
#define TFTFUNCTIONS_H

#ifdef __cplusplus
extern "C" {
#endif

void tft_sleep(void);
void tft_wakeup(void);
bool is_tft_sleeping();

void set_led_color_rgb(uint8_t r,uint8_t g,uint8_t b);
void set_led_color(lv_color32_t color);

#ifdef __cplusplus
} //extern "C" {
#endif


#endif