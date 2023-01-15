#include "menu.h"

MENU::MENU() {
	name = "MENU";
	map = create_map("Assets/map.txt");
}

SCENE* MENU::create() {
	MENU* temp = new MENU();
	return temp;
}

void MENU::update() {
}
void MENU::draw() {
	int W = al_get_bitmap_width(CONCEPT_DIAGRAM), H = al_get_bitmap_height(CONCEPT_DIAGRAM);
	al_draw_scaled_bitmap(
		CONCEPT_DIAGRAM,
		0, 0, W, H, 0, 0, W, H , 0
	);
}
void MENU::on_key_down(void) {
	if (key_down(ALLEGRO_KEY_ENTER))
		next_scene = NORMAL_STAGE::create();
}
void MENU::on_key_up(void) {
}
void MENU::on_mouse_down(void) {
}
void MENU::on_mouse_move(void) {
}
void MENU::on_mouse_up(void) {
}
void MENU::on_mouse_scroll(void) {
}