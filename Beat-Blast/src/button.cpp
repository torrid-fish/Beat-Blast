#include "button.h"

extern bool IS_2P;
extern bool LEVEL;

BUTTON::BUTTON(float x, float y, float w, float h, const char* name) {
    RECTANGLE temp(x, y, w, h);
	rec = temp;
	game_log("Button rec: ");
    printRecInfo(&rec);
    this->name = name;
    hovered = 0;
    color = al_map_rgb(200, 200, 200);
    hovered_color = al_map_rgb(150, 150, 150);
}

BUTTON::~BUTTON() {
    // destroy bitmap
}

void BUTTON::draw() {
	// ALLEGRO_BITMAP* _img = button.hovered_img ? 
	// 											button.hovered ? 
	// 												button.hovered_img : 
	// 												button.default_img : 
	// 											button.default_img;
	// al_draw_scaled_bitmap(
	// 	_img,
	// 	0, 0,
	// 	al_get_bitmap_width(_img), al_get_bitmap_height(_img),
	// 	button.body.x, button.body.y,
	// 	button.body.w, button.body.h, 0
	// );
	al_draw_filled_rectangle(
		draw_x + rec.x - 5, draw_y + rec.y - 5, draw_x + rec.x + rec.w + 5, draw_y + rec.y + rec.h + 5,
		color
	);
	al_draw_filled_rectangle(
		draw_x + rec.x, draw_y + rec.y, draw_x + rec.x + rec.w, draw_y + rec.y + rec.h,
		hovered ? color : hovered_color
	);
	const char* temp;
	if (!strcmp(name, "player"))
		temp = IS_2P ? "> 2P <" : "> 1P <";
	else if (!strcmp(name, "level"))
		temp = LEVEL ? "> HARD <" : "> EASY <";
	else	
		temp = name;
    al_draw_text(
        menuFont,
		al_map_rgb(255, 255, 255),
		draw_x + rec.midx(),
		draw_y + rec.midy()-10,
		ALLEGRO_ALIGN_CENTER,
		temp
    );
}

void BUTTON::update() {
    
}

bool BUTTON::buttonHover(int mouse_x, int mouse_y) {
	return pt_in_rect(mouse_x, mouse_y, rec);
}