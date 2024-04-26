#ifndef EMBEDDED_SKIN_H
#define  EMBEDDED_SKIN_H

#include "lvgl.h"

#ifdef __cplusplus
extern "C" {
#endif



extern const uint8_t NumberOfEmbeddedSkin;
extern const char* getEmbeddedSkinFile(uint8_t index);
extern const char* getEmbeddedSkinName(uint8_t index);

//* to get embedded image file, starting with "I:"
const lv_img_dsc_t* getEmbeddedImage(const char* filename);

#ifdef __cplusplus
} //extern "C" {
#endif

#endif