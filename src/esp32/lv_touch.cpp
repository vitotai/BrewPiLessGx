
#include <lvgl.h>
#include <Wire.h>
#include <Touch_GT911.h>
#include "lv_drv_conf.h"

#ifdef TOUCH_GT911
Touch_GT911 ts = Touch_GT911(TOUCH_GT911_SDA, TOUCH_GT911_SCL, TOUCH_GT911_INT, TOUCH_GT911_RST, TOUCH_MAP_MAX_X,  TOUCH_MAP_MAX_Y);
int touch_last_x = 0, touch_last_y = 0;
#define GT911_I2C_SLAVE_ADDR 0x5D
void touch_drv_init()
{
    Wire.begin(TOUCH_GT911_SDA, TOUCH_GT911_SCL);
    ts.begin(GT911_I2C_SLAVE_ADDR);
    ts.setRotation(TOUCH_GT911_ROTATION);
}


bool gt911_touch_touched()
{
  ts.read();
  if (ts.isTouched)
  {
    
#if TOUCH_SWAP_XY
    touch_last_x = ts.points[0].y;
    touch_last_y = ts.points[0].x;
#else
    touch_last_x = ts.points[0].x;
    touch_last_y = ts.points[0].y;
#endif

#if CONFIG_LV_GT911_INVERT_X
    touch_last_x = TOUCH_MAP_MAX_X - touch_last_x;
#endif
#if CONFIG_LV_GT911_INVERT_Y
    touch_last_y = TOUCH_MAP_MAX_Y - touch_last_y;
#endif

    return true;
  }
  else
  {
    return false;
  }
}


void touch_drv_read(lv_indev_drv_t *indev_driver, lv_indev_data_t *data)
{
    if (gt911_touch_touched())
    {
      data->state = LV_INDEV_STATE_PR;

      /*Set the coordinates*/
      data->point.x = touch_last_x;
      data->point.y = touch_last_y;
    }
    else 
    {
      data->state = LV_INDEV_STATE_REL;
    }
  
}

#endif //#ifdef TOUCH_GT911

#if TOUCH_INPUT_ENABLE
void touch_setup(void){
    touch_drv_init();
    /* Initialize the (dummy) input device driver */
    static lv_indev_drv_t indev_drv;
    lv_indev_drv_init(&indev_drv);
    indev_drv.type = LV_INDEV_TYPE_POINTER;
    indev_drv.read_cb = touch_drv_read;
    lv_indev_drv_register(&indev_drv);
}
#endif