#include "shared.h"
#include "utility.h"
#include "game_window.h"

ALLEGRO_FONT* font_pirulen_32;
ALLEGRO_FONT* font_pirulen_24;
ALLEGRO_SAMPLE* themeMusic = NULL;
ALLEGRO_SAMPLE* PACMAN_MOVESOUND = NULL;
ALLEGRO_SAMPLE* PACMAN_DEATH_SOUND = NULL;
ALLEGRO_SAMPLE* PACMAN_EAT_GHOST = NULL;
ALLEGRO_SAMPLE* powerupMusic = NULL;
ALLEGRO_FONT* menuFont = NULL;
ALLEGRO_SAMPLE_ID PACMAN_EAT_GHOST_ID;
ALLEGRO_SAMPLE_ID PACMAN_POWERUP_ID;
ALLEGRO_BITMAP* CONCEPT_DIAGRAM;

int fontSize = 30;
float music_volume = 0.5;
float effect_volume = 0.5;
bool gameDone = false;

void shared_init(void) {
	menuFont = load_font("Assets/Minecraft.ttf", fontSize);
	themeMusic = load_audio("Assets/Music/original_theme.ogg");
	PACMAN_MOVESOUND = load_audio("Assets/Music/pacman-chomp.ogg");
	PACMAN_DEATH_SOUND = load_audio("Assets/Music/pacman_death.ogg");
	powerupMusic = load_audio("Assets/Music/pacman_intermission.ogg");
	PACMAN_EAT_GHOST = load_audio("Assets/Music/pacman_eatghost.ogg");
	CONCEPT_DIAGRAM = load_bitmap("Assets/concept_diagram.png");
}

void shared_destroy(void) {
	al_destroy_font(menuFont);
	al_destroy_sample(themeMusic);
	al_destroy_sample(PACMAN_MOVESOUND);
	al_destroy_sample(PACMAN_DEATH_SOUND);
	al_destroy_sample(powerupMusic);
	al_destroy_sample(PACMAN_EAT_GHOST);
	al_destroy_bitmap(CONCEPT_DIAGRAM);
}