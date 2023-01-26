#include "crystal.h"

CRYSTAL::CRYSTAL(int grid_x, int grid_y, int element) {
    printRecInfo(&rec);
    this->grid_x = grid_x;
    this->grid_y = grid_y;
    this->grid_w = 1;
    this->grid_h = 1;
    drawrec = rec = get_rec_by_pt_on_board(grid_x, grid_y, grid_w, grid_h);
    this->name = "crystal";
	this->element = element; 
    beatCNT = 0;         		
    effecting = true;   
    color = al_map_rgb(150, 150, 200);

    sprite = load_bitmap("Assets/crystal.png");
}

CRYSTAL::~CRYSTAL() {
    // destroy bitmap
}

void CRYSTAL::draw() {
    /* Draw body */
    // al_draw_filled_circle(
    //     drawrec.midx(), drawrec.midy(), fmin(drawrec.w / 2, drawrec.h / 2),
    //     color
    // );
    
	float sp_w = al_get_bitmap_width(sprite);
    float sp_h = al_get_bitmap_height(sprite);

    al_draw_scaled_bitmap(
        sprite,
        0, 0,
        sp_w, sp_h,
        drawrec.x, drawrec.y,
        drawrec.w, drawrec.h,
        0
    );

    // Draw the loop
    al_draw_circle(
        drawrec.midx(), drawrec.midy(), fmin(drawrec.w / 2, drawrec.h / 2) * beatCNT / 16,
        color, 2
    );
}

void CRYSTAL::update() {
    // Update rectangle
    drawrec = rec = get_rec_by_pt_on_board(grid_x, grid_y, grid_w, grid_h);
}