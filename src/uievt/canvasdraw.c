
#include "canvasdraw.h"

void canvasDrawRectangle(lv_obj_t * canvas,lv_coord_t x, lv_coord_t y, lv_coord_t w, lv_coord_t h,lv_color_t color){

    for(int yv=y;yv < (y+h);yv++){
        for(int xv=x;xv < (x+w);xv++){
            lv_canvas_set_px(canvas,xv,yv,color);
        }
    }
}

void canvasDrawLine(lv_obj_t * canvas, lv_coord_t x1, lv_coord_t y1, lv_coord_t x2, lv_coord_t y2,lv_color_t color) {
    //int dx = abs(x2 - x1);
    //int dy = abs(y2 - y1);
    int dx = (x2>x1)? x2-x1:x1-x2;
    int dy= (y2>y1)? y2-y1:y1-y2;
    
    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? 1 : -1;
    int err = dx - dy;

    while (1) {
        // Plot the current pixel (x1, y1)
        lv_canvas_set_px(canvas,x1,y1,color);

        if (x1 == x2 && y1 == y2) {
            break;
        }

        int e2 = 2 * err;
        if (e2 > -dy) {
            err -= dy;
            x1 += sx;
        }
        if (e2 < dx) {
            err += dx;
            y1 += sy;
        }
    }
}

void canvasDrawCircle(lv_obj_t * canvas,int xc, int yc, int r, lv_color_t color) {
    int x = 0, y = r;
    int d = 3 - 2 * r;
    
    while (x <= y) {
        // Draw the eight symmetrical points
        lv_canvas_set_px(canvas,xc + x, yc + y,color);
        lv_canvas_set_px(canvas,xc - x, yc + y,color);
        lv_canvas_set_px(canvas,xc + x, yc - y,color);
        lv_canvas_set_px(canvas,xc - x, yc - y,color);
        lv_canvas_set_px(canvas,xc + y, yc + x,color);
        lv_canvas_set_px(canvas,xc - y, yc + x,color);
        lv_canvas_set_px(canvas,xc + y, yc - x,color);
        lv_canvas_set_px(canvas,xc - y, yc - x,color);

        if (d < 0) {
            d = d + 4 * x + 6;
        } else {
            d = d + 4 * (x - y) + 10;
            y--;
        }
        x++;
    }
}

static void lv_canvas_set_px_safe(lv_obj_t * obj,int x,int y,uint16_t w, uint16_t h,lv_color_t color) {
    if(x>=0 && x<w && y>=0 && y<h){
        //lv_canvas_set_px(canvas,x,y,color);        
        lv_canvas_t * canvas = (lv_canvas_t *)obj;
        lv_img_buf_set_px_color(&canvas->dsc, x, y, color);

    }
}

static void canvasDrawCircleSafe_vop(lv_obj_t * canvas,int xc, int yc, int r, lv_color_t color,bool invalidate) {

    int x = 0, y = r;
    int d = 3 - 2 * r;
    int w=lv_obj_get_width(canvas);
    int h=lv_obj_get_height(canvas);
    
    while (x <= y) {
        // Draw the eight symmetrical points
        lv_canvas_set_px_safe(canvas,xc + x, yc + y,w,h,color);
        lv_canvas_set_px_safe(canvas,xc - x, yc + y,w,h,color);
        lv_canvas_set_px_safe(canvas,xc + x, yc - y,w,h,color);
        lv_canvas_set_px_safe(canvas,xc - x, yc - y,w,h,color);
        lv_canvas_set_px_safe(canvas,xc + y, yc + x,w,h,color);
        lv_canvas_set_px_safe(canvas,xc - y, yc + x,w,h,color);
        lv_canvas_set_px_safe(canvas,xc + y, yc - x,w,h,color);
        lv_canvas_set_px_safe(canvas,xc - y, yc - x,w,h,color);

        if (d < 0) {
            d = d + 4 * x + 6;
        } else {
            d = d + 4 * (x - y) + 10;
            y--;
        }
        x++;
    }
    if(invalidate) lv_obj_invalidate(canvas);
}

void canvasDrawCircleSafe(lv_obj_t * canvas,int xc, int yc, int r, lv_color_t color) {
    canvasDrawCircleSafe_vop(canvas,xc,yc,r,color,true);
}
void canvasDrawCircleSafe_nv(lv_obj_t * canvas,int xc, int yc, int r, lv_color_t color) {
    canvasDrawCircleSafe_vop(canvas,xc,yc,r,color,false);
}
