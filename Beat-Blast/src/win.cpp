#include "win.h"

static ALLEGRO_SAMPLE_ID effect_id;
WIN::WIN() {
    name = "WIN";
	damping = false;
	draw_x = draw_y = 0;
	dampCNT = 0;
	dampThreshold = 1024;
    drawCNT = 0;
	effect_id = play_audio(YOU_WIN_EFFECT, effect_volume);
}

WIN::~WIN() {

}

void WIN::update() {
    drawCNT = (drawCNT + 1) % 1000;
}

void WIN::draw() {
    al_draw_text(
		menuFont,
		al_map_rgb(200, 200, 255),
		draw_x + SCREEN_W / 2, draw_y + SCREEN_H / 2,
		ALLEGRO_ALIGN_CENTER,
		"YOU WIN!"
	);
    al_draw_circle(
		draw_x + SCREEN_W / 2, draw_y + 0,
		drawCNT,
		al_map_rgb(230, 177, 167),
		drawCNT / 100
	);
	al_draw_circle(
		draw_x + SCREEN_W / 2, draw_y + SCREEN_H / 2,
		drawCNT,
		al_map_rgb(255, 255, 255),
		drawCNT / 100
	);
    al_draw_circle(
		draw_x + SCREEN_W / 2, draw_y + SCREEN_H,
		drawCNT,
		al_map_rgb(156, 209, 221),
		drawCNT / 100
	);
}

void WIN::on_key_change() {
    if (key_down(ALLEGRO_KEY_ENTER))
		next_scene = new MENU();
}

void WIN::on_mouse_down() {

}

void WIN::on_mouse_move() {

}

void WIN::on_mouse_up() {

}

void WIN::on_mouse_scroll() {

}