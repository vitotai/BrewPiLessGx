#include "rxlevel.h"


rxlevel_widget *lv_rxlevel_create(lv_obj_t *parent,uint16_t width,uint16_t height){
    rxlevel_widget *widget=(rxlevel_widget *)lv_mem_alloc(sizeof(rxlevel_widget));
    widget->width = width;
    widget->height = height;
    lv_obj_t *base = lv_obj_create(parent);
    widget->base = base;
    lv_obj_set_style_border_width(base,0,0);
    lv_obj_set_style_pad_all(base,0,0);
    lv_obj_set_style_radius(base,0,0);
    lv_obj_set_scrollbar_mode(base,LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_style_bg_opa(base,LV_OPA_0,0);
    //lv_obj_set_style_bg_color(base,lv_color_make(0,0xFF,0),0);
 
    lv_coord_t width_unit=  width /7 ;
    lv_coord_t height_unit= height /4;

    lv_coord_t x= 0;
    lv_coord_t y = widget->height - height_unit;
    lv_coord_t cheight = height_unit;

    for(int i=0;i<4;i++){
        lv_obj_t *bar= lv_obj_create(base);
        lv_obj_set_align(bar,LV_ALIGN_TOP_LEFT);
        lv_obj_set_pos(bar,x,y);
        lv_obj_set_size(bar,width_unit,cheight);
        lv_obj_set_style_border_width(bar,0,0);
        lv_obj_set_scrollbar_mode(bar,LV_SCROLLBAR_MODE_OFF);
        lv_obj_set_style_bg_color(bar,lv_color_white(),0);
        lv_obj_set_style_bg_opa(bar,LV_OPA_COVER,LV_PART_MAIN | LV_STATE_CHECKED);
        lv_obj_set_style_bg_opa(bar,LV_OPA_20,LV_PART_MAIN | LV_STATE_DEFAULT);

        x += 2 * width_unit;
        y -= height_unit;
        cheight += height_unit;

        widget->bars[i] = bar;
    }

    widget->disabled_label = lv_label_create(base);
    lv_label_set_text(widget->disabled_label,LV_SYMBOL_CLOSE);
    //lv_label_set_text(widget->disabled_label,"?");
    lv_obj_set_style_text_color(widget->disabled_label,lv_color_hex(0xFF00000),LV_STATE_DEFAULT | LV_PART_MAIN );
    //lv_obj_center(widget->disabled_label);
    return widget;
}

static void rxlevel_draw_level(rxlevel_widget* widget,uint8_t level){
    for(int i=0;i<4;i++){
        if(i < level){
            lv_obj_add_state(widget->bars[i],LV_STATE_CHECKED);
        }else{
            lv_obj_clear_state(widget->bars[i],LV_STATE_CHECKED);
        }
    }
}

void lv_rxlevel_setLevel(rxlevel_widget* widget,uint8_t level){
    if(level == widget->level) return;    
    widget->level=level;
    // if the widget is "disabled", don't redraw
    rxlevel_draw_level(widget,level);
}

void lv_rxlevel_setDisabled(rxlevel_widget* widget,bool disabled){
    if(widget->disabled == disabled) return;
    widget->disabled = disabled;
    if(disabled){
        rxlevel_draw_level(widget,0);
        lv_obj_clear_flag(widget->disabled_label,LV_OBJ_FLAG_HIDDEN);
    }else{
        rxlevel_draw_level(widget,widget->level);
        lv_obj_add_flag(widget->disabled_label,LV_OBJ_FLAG_HIDDEN);
    }
}
/*
void lv_rxlevel_setBgColor(rxlevel_widget* widget,lv_color_t color){
}
*/
void lv_rxlevel_setColor(rxlevel_widget* widget,lv_color_t color){
    for(int i=0;i<4;i++){
        lv_obj_set_style_bg_color(widget->bars[i],color,0);
    }
}
/*
void lv_rxlevel_setSubColor(rxlevel_widget* widget,lv_color_t color){
  //  for(int i=0;i<4;i++){
  //  }
}
*/

lv_obj_t *lv_rxlevel_getCanvas(rxlevel_widget* widget){
    return widget->base;
}
