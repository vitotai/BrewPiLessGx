#include "embedded_skin.h"

#define EMBEDDED_IMG_PREFIX 'I'

typedef struct _SkinEntry{
    const char* name;
    const char* file;
} SkinEntry;
#if SKIN320170
extern const uint8_t skin_brwpi_start[] asm("_binary_src_skins_skin_brewpi3217_json_start");
extern const uint8_t skin_hydrometer_start[] asm("_binary_src_skins_skin_hydro3217_json_start");

static SkinEntry embeddedSkins[]={
{"BrwPi",skin_brwpi_start},
{"Hydrometer",skin_hydrometer_start}
};
#endif

#if SKIN320240
extern const uint8_t skin_brwpi_start[] asm("_binary_src_skins_skin_brewpi3224_json_start");
extern const uint8_t skin_info_start[] asm("_binary_src_skins_skin_info3224_json_start");

static SkinEntry embeddedSkins[]={
{"BrwPi",skin_brwpi_start},
{"Infomation",skin_info_start}
};
#endif

#if SKIN480320
extern const uint8_t skin_brwpi_start[] asm("_binary_src_skins_skin_brewpi4832_json_start");
extern const uint8_t skin_info_start[] asm("_binary_src_skins_skin_info4832_json_start");

static SkinEntry embeddedSkins[]={
{"BrwPi",skin_brwpi_start},
{"Infomation",skin_info_start}
};

#endif

const uint8_t NumberOfEmbeddedSkin=sizeof(embeddedSkins)/sizeof(SkinEntry);

const char* getEmbeddedSkinFile(uint8_t index){
    if(index < NumberOfEmbeddedSkin) return embeddedSkins[index].file;
    return "";
}
const char* getEmbeddedSkinName(uint8_t index){
    if(index < NumberOfEmbeddedSkin) return embeddedSkins[index].name;
    return "Invalid";
}


//******************************* 
// image files
typedef struct _EmbeddedFileImageEntry{
const lv_img_dsc_t *img_dsc;
const char *filename;
}EmbeddedFileImageEntry;


LV_IMG_DECLARE(state_icons_4bit);

static EmbeddedFileImageEntry _imgFiles[]={
{&state_icons_4bit,"istate"}
};

const lv_img_dsc_t* getEmbeddedImage(const char* filename){
    if(filename[0] != EMBEDDED_IMG_PREFIX){
        return NULL;
    }
    for(int i=0;i<sizeof(_imgFiles)/sizeof(EmbeddedFileImageEntry);i++){
        if(strcmp(filename+2,_imgFiles[i].filename)==0){
            return _imgFiles[i].img_dsc;
        }
    }
    return NULL;
}

