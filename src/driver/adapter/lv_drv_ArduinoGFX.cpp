
#include <lvgl.h>
#ifdef ArduinoGFX_Library
#include <Arduino_GFX_Library.h>
#include "lv_drv_conf.h"

/* More dev device declaration: https://github.com/moononournation/Arduino_GFX/wiki/Dev-Device-Declaration */

#if ESP32_17320S019N
#define GFX_BL 14
Arduino_DataBus *bus = new Arduino_ESP32SPI(11 /* DC */, 10 /* CS */, 12 /* SCK */, 13 /* MOSI */, GFX_NOT_DEFINED /* MISO */);
Arduino_GFX *gfx = new Arduino_ST7789(bus, 1 /* RST */, 1 /* rotation */, true /* IPS */, 170 /* width */, 320 /* height */, 35 /* col offset 1 */, 0 /* row offset 1 */, 35 /* col offset 2 */, 0 /* row offset 2 */);
#define DISPLAY_BUS_SPEED 80000000
#endif

#if ESP32_2432S032C
#define GFX_BL 27
Arduino_DataBus *bus = new Arduino_ESP32SPI(2 /* DC */, 15 /* CS */, 14 /* SCK */, 13 /* MOSI */, 12 /* MISO */);
Arduino_GFX *gfx = new Arduino_ILI9341(bus, GFX_NOT_DEFINED /* RST */, 1 /* rotation */);
#define DISPLAY_BUS_SPEED 50000000
#endif


#define TFT_PWM_CHANNEL_BL 7
#define TFT_PWM_FREQ_BL 5000
#define TFT_PWM_BITS_BL 8
#define TFT_PWM_MAX_BL ((1 << TFT_PWM_BITS_BL) - 1)

#ifdef __cplusplus
extern "C" {
#endif
void display_drv_sleep(void);
void display_drv_wakeup(void);
#ifdef __cplusplus
}
#endif

/*******************************************************************************
 * End of Arduino_GFX setting
 ******************************************************************************/

void display_drv_sleep(void){
   gfx->displayOff();
}
void display_drv_wakeup(void){
   gfx->displayOn();
}


/* Change to your screen resolution */

/* Display flushing */
void display_drv_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p)
{
   uint32_t w = (area->x2 - area->x1 + 1);
   uint32_t h = (area->y2 - area->y1 + 1);

#if (LV_COLOR_16_SWAP != 0)
   gfx->draw16bitBeRGBBitmap(area->x1, area->y1, (uint16_t *)&color_p->full, w, h);
#else
   gfx->draw16bitRGBBitmap(area->x1, area->y1, (uint16_t *)&color_p->full, w, h);
#endif

   lv_disp_flush_ready(disp);
}

void display_drv_init()
{
   // Init Display
   gfx->begin(DISPLAY_BUS_SPEED); /* specify data bus speed */
   gfx->fillScreen(BLACK);

#ifdef GFX_BL
   pinMode(GFX_BL, OUTPUT);
   digitalWrite(GFX_BL, HIGH);
   ledcSetup(TFT_PWM_CHANNEL_BL, TFT_PWM_FREQ_BL, TFT_PWM_BITS_BL);
   ledcAttachPin(GFX_BL, TFT_PWM_CHANNEL_BL);
   ledcWrite(TFT_PWM_CHANNEL_BL, TFT_PWM_MAX_BL); /* Screen brightness can be modified by adjusting this parameter. (0-255) */
#endif
}
void display_drv_set_brightness(uint8_t brightness){
#ifdef GFX_BL
   ledcWrite(TFT_PWM_CHANNEL_BL, brightness); /* Screen brightness can be modified by adjusting this parameter. (0-255) */
#endif
}
#endif