#ifndef SCENE_SHARED_H
#define SCENE_SHARED_H
#define _CRT_SECURE_NO_DEPRECATE
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_audio.h>

extern ALLEGRO_FONT* font_pirulen_32;
extern ALLEGRO_FONT* font_pirulen_24;
extern ALLEGRO_SAMPLE* themeMusic;
extern ALLEGRO_SAMPLE* PACMAN_MOVESOUND;
extern ALLEGRO_SAMPLE* PACMAN_DEATH_SOUND;
extern ALLEGRO_FONT* menuFont;
extern ALLEGRO_BITMAP* CONCEPT_DIAGRAM;
extern int fontSize;
extern float music_volume;
extern float effect_volume;
extern bool gameDone;

void shared_init(void);
void shared_destroy(void);

#endif