#ifndef TFTFUNCTIONS_H
#define TFTFUNCTIONS_H

#ifdef __cplusplus
extern "C" {
#endif

void tft_sleep(void);
void tft_wakeup(void);
bool is_tft_sleeping();


#ifdef __cplusplus
} //extern "C" {
#endif


#endif