
#ifndef LV_DRV_CONF_H
#define LV_DRV_CONF_H


// Display driver Configuration

#if ESP32_17320S019N
#define CONFIG_LVGL_DISPLAY_WIDTH 320
#define CONFIG_LVGL_DISPLAY_HEIGHT 170
// size of lv_color_t
// should be greater than 1/10 whole screen.
#define LV_DRAW_BUFFER_SIZE (CONFIG_LVGL_DISPLAY_WIDTH * 10)


//#define TFT_PWM_CHANNEL_BL 0
//#define TFT_PWM_FREQ_BL 2000
//#define TFT_PWM_BITS_BL 8

#endif // ESP32_17320S019N

#ifdef ESP32_2432S032C

#define CONFIG_LVGL_DISPLAY_WIDTH 320
#define CONFIG_LVGL_DISPLAY_HEIGHT 240
// size of lv_color_t
// should be greater than 1/10 whole screen.
#define LV_DRAW_BUFFER_SIZE (CONFIG_LVGL_DISPLAY_WIDTH * 10)


// Touch driver
#define  TOUCH_INPUT_ENABLE true
 #define TOUCH_GT911_SCL 32
 #define TOUCH_GT911_SDA 33
 #define TOUCH_GT911_INT -1
 #define TOUCH_GT911_RST 25
 #define TOUCH_GT911_ROTATION ROTATION_RIGHT//ROTATION_NORMAL
 #define TOUCH_MAP_MAX_X CONFIG_LVGL_DISPLAY_WIDTH
 #define TOUCH_MAP_MAX_Y CONFIG_LVGL_DISPLAY_HEIGHT
// #define TOUCH_SWAP_XY true

#endif // ESP32_2432S032C

#ifdef ESP32_2432S028R
#define CONFIG_LVGL_DISPLAY_WIDTH 320
#define CONFIG_LVGL_DISPLAY_HEIGHT 240
// size of lv_color_t
// should be greater than 1/10 whole screen.
#define LV_DRAW_BUFFER_SIZE (CONFIG_LVGL_DISPLAY_WIDTH * 10)

#endif // ESP32_2432S028R


#endif