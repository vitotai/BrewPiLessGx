#include <Arduino.h>
#include "wakebutton.h"

#if  WAKEUP_BUTTON_ENABLED


void wakebutton_init(void){
    pinMode(WAKEUP_PIN, INPUT_PULLUP);
}

bool wakebutton_pressed(void){
    return digitalRead(WAKEUP_PIN) == 0;
}



#endif