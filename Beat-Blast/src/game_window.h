#define _CRT_SECURE_NO_WARNINGS
#ifndef GAME_WINDOW_H
#define GAME_WINDOW_H
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>	
#include <string>
#include "scene.h"
using namespace std;
#define LOG_ENABLED

/* Global variables. */
// Frame rate (frame per second)
extern const int FPS;
// Display (screen) width.
extern const int SCREEN_W;
// Display (screen) height.
extern const int SCREEN_H;
// At most 4 audios can be played at a time.
extern const int RESERVE_SAMPLES;

// Receive change scene signal.
extern SCENE* next_scene;

extern uint32_t			GAME_TICK;
extern uint32_t			SLOWER_GAME_TICK;
extern const uint32_t	GAME_TICK_CD;
extern const uint32_t	SLOWER_GAME_TICK_CD;

/* Fetch font/music initialization. */

// Initialize shared variables and resources.
// Allows the game to perform any initialization it needs before
// starting to run.
extern void shared_init(void);
// Free shared variables and resources.
extern void shared_destroy(void);

// Top Class for game update
class GAME_WINDOW {
public:
	GAME_WINDOW();						// Constructer
	~GAME_WINDOW();						// Destructer
	void game_start();					// Game start
	void game_change_scene(SCENE*);		// Change from one scene to another
private:
	void event_loop();					// Event loop
	void game_init();					// Initialize the game
	void allegro5_init();				// Initialization regarding allegro5
	const char* game_title;				// The title of our game
	ALLEGRO_DISPLAY* display;			// The screen for display
	ALLEGRO_TIMER* flip_timer;			// Update: (1 / FPS). For draw
	ALLEGRO_TIMER* tick_timer;			// Update: (1 / GAME_TICK_CD / 2). For update
	ALLEGRO_EVENT_QUEUE* event_queue;	// Event queue of game
	SCENE* current_scene;				// Current used scene
	bool game_done;						// Whether the game has done
};

extern GAME_WINDOW beat_blast;

#endif