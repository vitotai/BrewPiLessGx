#ifndef SCREENSAVER_H
#define SCREENSAVER_H

#ifdef __cplusplus
extern "C" {
#endif

#include "lvgl.h"

void screenSaver_create(lv_obj_t* parent);

void screenSaver_start(lv_obj_t* parent);
void onScreenSaverClicked(lv_event_t * e);

void periodic_timer_handler(lv_timer_t *timer);
void screenSaverInitDraw(void);
void screenSaverUpdate(void);

void userStopSaver(void);

void displayWakeup(void);

#ifdef __cplusplus
} //extern "C" {
#endif


#endif