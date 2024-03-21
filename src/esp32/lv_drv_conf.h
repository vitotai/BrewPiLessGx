
#ifndef LV_DRV_CONF_H
#define LV_DRV_CONF_H


// Display driver Configuration

#if ESP32_17320S019N
#define CONFIG_LVGL_DISPLAY_WIDTH 320
#define CONFIG_LVGL_DISPLAY_HEIGHT 170
// size of lv_color_t
// should be greater than 1/10 whole screen.
#define LV_DRAW_BUFFER_SIZE (CONFIG_LVGL_DISPLAY_WIDTH * 10)


#define TFT_BUS_SPI

#define TFT_SPI_DC_PIN 11
#define TFT_SPI_CS_PIN 10
#define TFT_SPI_SCK_PIN 12
#define TFT_SPI_MOSI 13
#define SPI_MISO GFX_NOT_DEFINED

#define TFT_ST7789
#define TFT_RST_PIN 1
#define TFT_ROTATION 1
#define TFT_IPS true
#define TFT_DRV_WIDTH 170
#define TFT_DRV_HEIGHT 320
#define TFT_COL_OFFSET_1 35
#define TFT_ROW_OFFSET_1 0
#define TFT_COL_OFFSET_2 35
#define TFT_ROW_OFFSET_2 0

#define TFT_PWM_CHANNEL_BL 0
#define TFT_PWM_FREQ_BL 2000
#define TFT_PWM_BITS_BL 8


#define GFX_BL DF_GFX_BL // default backlight pin, you may replace DF_GFX_BL to actual backlight pin
#define TFT_BL_PIN 14

#define DISPLAY_BUS_SPEED 80000000

#endif // ESP32_17320S019N

#ifdef ESP32_2432S032C

#define CONFIG_LVGL_DISPLAY_WIDTH 320
#define CONFIG_LVGL_DISPLAY_HEIGHT 240
// size of lv_color_t
// should be greater than 1/10 whole screen.
#define LV_DRAW_BUFFER_SIZE (CONFIG_LVGL_DISPLAY_WIDTH * 10)

#define TFT_BUS_SPI

#define TFT_SPI_DC_PIN 2
#define TFT_SPI_CS_PIN 15
#define TFT_SPI_SCK_PIN 14
#define TFT_SPI_MOSI 13
#define TFT_SPI_MISO 12

#define TFT_DRV_WIDTH CONFIG_LVGL_DISPLAY_WIDTH
#define TFT_DRV_HEIGHT CONFIG_LVGL_DISPLAY_HEIGHT

#define ILI9341
#define DISPLAY_BUS_SPEED 50000000

#define TFT_RST_PIN GFX_NOT_DEFINED
#define TFT_ROTATION 1
#define TFT_IPS false


#define TFT_PWM_CHANNEL_BL 7
#define TFT_PWM_FREQ_BL 5000
#define TFT_PWM_BITS_BL 8

#define TFT_BL_PIN 27

// Touch driver
#define  TOUCH_INPUT_ENABLE true
 #define TOUCH_GT911
 #define TOUCH_GT911_SCL 32
 #define TOUCH_GT911_SDA 33
 #define TOUCH_GT911_INT -1
 #define TOUCH_GT911_RST 25
 #define TOUCH_GT911_ROTATION ROTATION_RIGHT//ROTATION_NORMAL
 #define TOUCH_MAP_MAX_X CONFIG_LVGL_DISPLAY_WIDTH
 #define TOUCH_MAP_MAX_Y CONFIG_LVGL_DISPLAY_HEIGHT
// #define TOUCH_SWAP_XY true

#endif // ESP32_2432S032C

#endif