#include "shared.h"
#include "utility.h"
#include "game_window.h"

ALLEGRO_FONT* font_pirulen_32;
ALLEGRO_FONT* font_pirulen_24;
ALLEGRO_SAMPLE* THE_BRUH_EFFECT = NULL;
ALLEGRO_SAMPLE* SAMSONG_NOTIFICATION_EFFECT = NULL;
ALLEGRO_SAMPLE* GAME_BGM = NULL;
ALLEGRO_SAMPLE* ENTER_INTRO = NULL;
ALLEGRO_SAMPLE* DAMAGED_EFFECT = NULL;
ALLEGRO_SAMPLE* YOU_WIN_EFFECT = NULL;
ALLEGRO_FONT* menuFont = NULL;
ALLEGRO_BITMAP* CONCEPT_DIAGRAM = NULL;
ALLEGRO_BITMAP* stone_ground = NULL;
ALLEGRO_BITMAP* stone_brick = NULL;
ALLEGRO_BITMAP* exit_block = NULL;
ALLEGRO_BITMAP* p1_script = NULL;
ALLEGRO_BITMAP* p2_script = NULL;

int fontSize = 30;
float music_volume = 0.5;
float effect_volume = 0.5;
bool gameDone = false;

void shared_init(void) {
	menuFont = load_font("Assets/Minecraft.ttf", fontSize);
	THE_BRUH_EFFECT = load_audio("Assets/Music/the_bruh_sound.ogg");
	SAMSONG_NOTIFICATION_EFFECT = load_audio("Assets/Music/samsong_notification.ogg");
	GAME_BGM = load_audio("Assets/Music/game_bgm.ogg");
	ENTER_INTRO = load_audio("Assets/Music/enter_intro.ogg");
	DAMAGED_EFFECT = load_audio("Assets/Music/damage_effect.ogg");
	CONCEPT_DIAGRAM = load_bitmap("Assets/concept_diagram.png");
	YOU_WIN_EFFECT = load_audio("Assets/Music/you_win.ogg");
	stone_ground = load_bitmap("Assets/floor.png");
	stone_brick = load_bitmap("Assets/stone.png");
	exit_block = load_bitmap("Assets/exit.png");
	p1_script = load_bitmap("Assets/player_down.png");
	p2_script = load_bitmap("Assets/player2_down.png");
}

void shared_destroy(void) {
	al_destroy_font(menuFont);
	al_destroy_sample(THE_BRUH_EFFECT);
	al_destroy_sample(SAMSONG_NOTIFICATION_EFFECT);
	al_destroy_sample(DAMAGED_EFFECT);
	al_destroy_sample(GAME_BGM);
	al_destroy_sample(ENTER_INTRO);
	al_destroy_sample(YOU_WIN_EFFECT);
	al_destroy_bitmap(stone_ground);
	al_destroy_bitmap(stone_brick);
	al_destroy_bitmap(exit_block);
	al_destroy_bitmap(CONCEPT_DIAGRAM);
}