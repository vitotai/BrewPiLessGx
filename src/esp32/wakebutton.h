#ifndef WAKEBUTTON_H
#define WAKEBUTTON_H
#ifdef __cplusplus
extern "C" {
#endif

#define WAKEUP_PIN 0


void wakebutton_init(void);
bool wakebutton_pressed(void);


void screenWakeup(void);
#ifdef __cplusplus
}
#endif

#endif