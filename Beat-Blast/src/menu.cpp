#include "menu.h"

extern int mouse_x, mouse_y;

// Mouse state, whether the key is down or not.
// 1 is for left, 2 is for right, 3 is for middle.
extern bool* mouse_state;
extern bool IS_2P;
extern bool LEVEL;

MENU::MENU() {
	name = "MENU";
	map = create_map("Assets/map.txt");
	// Damping initialization
	damping = false;
	draw_x = draw_y = 0;
	dampCNT = 0;
	dampThreshold = 1024;
	// Button initialization
	button_start = new BUTTON(SCREEN_W / 2 - 100, SCREEN_H / 2 - 25, 200, 50, "-START-");
	buttons.emplace_back(button_start);
	button_oi = new BUTTON(SCREEN_W / 2 - 100, SCREEN_H / 2 + 100 - 25, 200, 50, "player");
	buttons.emplace_back(button_oi);
	button_level = new BUTTON(SCREEN_W / 2 - 100, SCREEN_H / 2 + 200 - 25, 200, 50, "level");
	buttons.emplace_back(button_level);
	IS_2P = true;
	LEVEL = false;
	// Damping intro
	damping = true, dampCNT = 0;
}

void MENU::update() {
	// Damping process
	if (damping) {
		dampCNT += 1;
		float damped_val = dampedOscillation(500, dampCNT, pi / 64, 0.01);
		if (dampCNT > dampThreshold) damping = false;
		draw_x = damped_val, draw_y = 0;
	}
	else {
		draw_x = draw_y = 0;
	}
}
void MENU::draw() {
	// int W = al_get_bitmap_width(CONCEPT_DIAGRAM), H = al_get_bitmap_height(CONCEPT_DIAGRAM);
	// al_draw_scaled_bitmap(
	// 	CONCEPT_DIAGRAM,
	// 	draw_x, draw_y, W, H, 0, 0, W, H , 0
	// );
	int p1_x, p1_y, p2_x, p2_y;
	for (int col = 0; col < map->col_num; col++)
		for (int row = 0; row < map->row_num; row++) {
			if (is_p1(map, col, row))
				p1_x = col, p1_y = row;
			if (is_p2(map, col, row))
				p2_x = col, p2_y = row;
		}
	
	RECTANGLE rec = get_rec_by_pt_on_board(p1_x, p1_y, 2, 2);

	float sp_w = al_get_bitmap_width(p1_script);
    float sp_h = al_get_bitmap_height(p1_script);

    al_draw_scaled_bitmap(
        p1_script,
        0, 0,
        sp_w, sp_h,
		draw_x + rec.x, draw_y + rec.y,
        rec.w, rec.h, 
        0
    );

	if (IS_2P) {
		RECTANGLE rec2 = get_rec_by_pt_on_board(p2_x, p2_y, 2, 2);

		float sp_w = al_get_bitmap_width(p2_script);
		float sp_h = al_get_bitmap_height(p2_script);

		al_draw_scaled_bitmap(
			p2_script,
			0, 0,
			sp_w, sp_h,
			draw_x + rec2.x, draw_y + rec2.y,
			rec2.w, rec2.h, 
			0
		);
	}

	al_draw_text(
		menuFont,
		al_map_rgb(200, 200, 255),
		draw_x + SCREEN_W / 2, draw_y + 80,
		ALLEGRO_ALIGN_CENTER,
		"BEAT BLAST"
	);

	for (auto button : buttons) {
		button->draw_x = draw_x;
		button->draw_y = draw_y;
		button->draw();
	}
}
void MENU::on_key_change(void) {
	if (key_down(ALLEGRO_KEY_ENTER))
		next_scene = new NORMAL_STAGE();
	else if (key_down(ALLEGRO_KEY_SPACE))
		damping = true, dampCNT = 0;
	else if (key_down(ALLEGRO_KEY_1))
		IS_2P = false;
	else if (key_down(ALLEGRO_KEY_2))
		IS_2P = true;
	else if (key_down(ALLEGRO_KEY_H))
		LEVEL ^= 1;
}
void MENU::on_mouse_down(void) {
	if (button_start->hovered && mouse_state[1])
		next_scene = new NORMAL_STAGE();
	if (button_oi->hovered && mouse_state[1])
		dampCNT = 0, IS_2P ^= 1;
	if (button_level->hovered && mouse_state[1])
		dampCNT = 0, LEVEL ^= 1;
}
void MENU::on_mouse_move(void) {
	for (auto button: buttons) {
		button->hovered = button->buttonHover(mouse_x, mouse_y);
		// game_log("%d", button->hovered);
	}
}
void MENU::on_mouse_up(void) {
}
void MENU::on_mouse_scroll(void) {
}