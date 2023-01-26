#ifndef SCENE_SHARED_H
#define SCENE_SHARED_H
#define _CRT_SECURE_NO_DEPRECATE
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_audio.h>

extern ALLEGRO_FONT* font_pirulen_32;
extern ALLEGRO_FONT* font_pirulen_24;
extern ALLEGRO_SAMPLE* THE_BRUH_EFFECT;
extern ALLEGRO_SAMPLE* SAMSONG_NOTIFICATION_EFFECT;
extern ALLEGRO_SAMPLE* GAME_BGM;
extern ALLEGRO_SAMPLE* ENTER_INTRO;
extern ALLEGRO_SAMPLE* DAMAGED_EFFECT;
extern ALLEGRO_FONT* menuFont;
extern ALLEGRO_BITMAP* CONCEPT_DIAGRAM;
extern ALLEGRO_SAMPLE* YOU_WIN_EFFECT;
extern int fontSize;
extern float music_volume;
extern float effect_volume;
extern bool gameDone;

extern ALLEGRO_BITMAP* stone_ground;
extern ALLEGRO_BITMAP* stone_brick;
extern ALLEGRO_BITMAP* exit_block;
extern ALLEGRO_BITMAP* p1_script;
extern ALLEGRO_BITMAP* p2_script;


void shared_init(void);
void shared_destroy(void);

#endif