
#include <lvgl.h>
#include <Arduino_GFX_Library.h>
#include "lv_drv_conf.h"

#define TFT_PWM_MAX_BL ((1 << TFT_PWM_BITS_BL) - 1)

/* More dev device declaration: https://github.com/moononournation/Arduino_GFX/wiki/Dev-Device-Declaration */

#ifdef TFT_BUS_SPI
Arduino_DataBus *bus = new Arduino_ESP32SPI( SPI_DC, SPI_CS, SPI_SCK, SPI_MOSI, SPI_MISO);
#endif

#ifdef TFT_ST7789
Arduino_GFX *gfx = new Arduino_ST7789(bus, TFT_RST, TFT_ROTATION, TFT_IPS,TFT_WIDTH, TFT_HEIGHT,TFT_COL_OFFSET_1,TFT_ROW_OFFSET_1, TFT_COL_OFFSET_2,TFT_ROW_OFFSET_2);
#endif

#ifdef ILI9341
Arduino_GFX *gfx = new Arduino_ILI9341(bus,TFT_RST,TFT_ROTATION,TFT_IPS);
#endif
#define BUFFER_SIZE (320 *10)
/*******************************************************************************
 * End of Arduino_GFX setting
 ******************************************************************************/

/* Change to your screen resolution */
static lv_disp_draw_buf_t draw_buf;
static lv_color_t *disp_draw_buf;
static lv_disp_drv_t disp_drv;

/* Display flushing */
void my_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p)
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

void display_setup()
{

   // Init Display
   //gfx->begin();
   gfx->begin(DISPLAY_BUS_SPEED); /* specify data bus speed */
   gfx->fillScreen(BLACK);

#ifdef TFT_BL
   pinMode(TFT_BL, OUTPUT);
   digitalWrite(TFT_BL, HIGH);
#endif
   //ledcSetup(0, 2000, 8);
   //ledcAttachPin(TFT_BL, 0);
  ledcSetup(TFT_PWM_CHANNEL_BL, TFT_PWM_FREQ_BL, TFT_PWM_BITS_BL);
  ledcAttachPin(TFT_BL, TFT_PWM_CHANNEL_BL);

   ledcWrite(TFT_PWM_CHANNEL_BL, TFT_PWM_MAX_BL); /* Screen brightness can be modified by adjusting this parameter. (0-255) */
   lv_init();

   uint32_t screenWidth = gfx->width();
   uint32_t screenHeight = gfx->height();
#ifdef ESP32
   disp_draw_buf = (lv_color_t *)heap_caps_malloc(sizeof(lv_color_t) * LV_DRAW_BUFFER_SIZE, MALLOC_CAP_INTERNAL | MALLOC_CAP_8BIT);
#else
   disp_draw_buf = (lv_color_t *)malloc(sizeof(lv_color_t) * LV_DRAW_BUFFER_SIZE);
#endif
   if (!disp_draw_buf)
   {
      Serial.println("LVGL disp_draw_buf allocate failed!");
   }
   else
   {
      lv_disp_draw_buf_init(&draw_buf, disp_draw_buf, NULL, LV_DRAW_BUFFER_SIZE);
      /* Initialize the display */
      lv_disp_drv_init(&disp_drv);
      /* Change the following line to your display resolution */
      disp_drv.hor_res = screenWidth;
      disp_drv.ver_res = screenHeight;
      disp_drv.flush_cb = my_disp_flush;
      disp_drv.draw_buf = &draw_buf;
      lv_disp_drv_register(&disp_drv);
      
      lv_obj_clean(lv_scr_act());
   }
}
