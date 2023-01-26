#include "item.h"

ITEM::ITEM(float x, float y, float w, float h, int effect) {
    this->x = x,
    this->y = y;
    this->w = w;
    this->h = h;
    RECTANGLE temp(x, y, w, h);
    rec = temp;
    printRecInfo(&rec);
    this->name = "ITEM";        		
    effect = 0;
    color = al_map_rgb(220, 220, 255);
}

ITEM::~ITEM() {
    // destroy bitmap
}

void ITEM::draw() {
    float damping_x = map_offset_x - fix_map_offset_x, damping_y = map_offset_y - fix_map_offset_y;
    al_draw_filled_circle(
        rec.midx() + damping_x, rec.midy() + damping_y, fmin(rec.w / 2, rec.h / 2),
        color
    );
}

void ITEM::update() {
    
}