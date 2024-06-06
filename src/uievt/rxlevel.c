#include "rxlevel.h"
#include "canvasdraw.h"

#define COLOR_INDEX_BACKGROUND 0
#define COLOR_INDEX_WHITE 1
#define COLOR_INDEX_GREY 2
#define COLOR_INDEX_RED 3

static void clearBackground(rxlevel_widget* widget){
    lv_color_t c1;
    c1.full= COLOR_INDEX_BACKGROUND;
    lv_canvas_fill_bg(lv_rxlevel_getCanvas(widget),c1,LV_OPA_0);
}


static void drawRxLevel(rxlevel_widget* widget,uint8_t barNum){
    lv_obj_t *canvas = widget->canvas;

    lv_coord_t hu = widget->height / 4;
    lv_coord_t wu = widget->width / 7;

    lv_coord_t x= 0;
    lv_coord_t y = widget->height - hu;
    lv_coord_t width= wu;
    lv_coord_t height=hu;

    lv_color_t white;
    white.full = COLOR_INDEX_WHITE;
    lv_color_t grey;
    grey.full = COLOR_INDEX_GREY;

    for(int i=0;i<4;i++){
        //lv_canvas_draw_rect(canvas, x, y, width, height, &draw_dsc);
        canvasDrawRectangle(canvas, x, y, width, height, (i < barNum)? white:grey);
        x += 2 * wu;
        y -= hu;
        height += hu;
    }
}


static void drawDisabledSign(rxlevel_widget* widget){

    drawRxLevel(widget,0);
    lv_color_t color;
    color.full = COLOR_INDEX_RED;

    lv_coord_t w = widget->width;
    lv_coord_t h = widget->height;
    uint16_t r = (w>h? h/2:w/2); 
    r=(r<2)? r:r-2;
    uint16_t delta = r/1.414;

    canvasDrawCircle(widget->canvas,w/2,h/2,r,color);

    canvasDrawLine(widget->canvas,w/2 - delta , h/2 - delta,w/2 + delta,h/2 + delta, color);
}


static void drawRxLevelWidget(rxlevel_widget* widget){
    clearBackground(widget);

    if(widget->disabled) drawDisabledSign(widget);
    else drawRxLevel(widget,widget->level);
}

rxlevel_widget *lv_rxlevel_create(lv_obj_t *parent,uint16_t width,uint16_t height){
    rxlevel_widget *widget=(rxlevel_widget *)lv_mem_alloc(sizeof(rxlevel_widget));
    widget->width = width;
    widget->height = height;
    widget->buffer = (lv_color_t*) lv_mem_alloc(sizeof(lv_color_t)  *LV_CANVAS_BUF_SIZE_INDEXED_2BIT(width, height));

    lv_obj_t *canvas = lv_canvas_create(parent);
    widget->canvas = canvas;

    lv_canvas_set_buffer(canvas, widget->buffer, width, height, LV_IMG_CF_INDEXED_2BIT);
    lv_canvas_set_palette(canvas,COLOR_INDEX_BACKGROUND,lv_color_black());
    lv_canvas_set_palette(canvas,COLOR_INDEX_WHITE,lv_color_white());
    lv_canvas_set_palette(canvas,COLOR_INDEX_GREY,lv_color_make(0x04,0x04,0x04));
    lv_canvas_set_palette(canvas,COLOR_INDEX_RED,lv_color_make(0xFF,0x33,0x33));

    return widget;
}


void lv_rxlevel_setLevel(rxlevel_widget* widget,uint8_t level){
    if(level == widget->level) return;    
    widget->level=level;
    // if the widget is "disabled", don't redraw
    if(! widget->disabled) {
        drawRxLevelWidget(widget);
    }
}

void lv_rxlevel_setDisabled(rxlevel_widget* widget,bool disabled){
    if(widget->disabled == disabled) return;
    widget->disabled = disabled;
    drawRxLevelWidget(widget);
}

void lv_rxlevel_setBgColor(rxlevel_widget* widget,lv_color_t color){
    lv_canvas_set_palette(widget->canvas,COLOR_INDEX_BACKGROUND,color);
}

void lv_rxlevel_setColor(rxlevel_widget* widget,lv_color_t color){
    lv_canvas_set_palette(widget->canvas,COLOR_INDEX_WHITE,color);
}

void lv_rxlevel_setSubColor(rxlevel_widget* widget,lv_color_t color){
    lv_canvas_set_palette(widget->canvas,COLOR_INDEX_GREY,color);
}


lv_obj_t *lv_rxlevel_getCanvas(rxlevel_widget* widget){
    return widget->canvas;
}
