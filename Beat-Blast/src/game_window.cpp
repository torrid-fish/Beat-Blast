#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include "game_window.h"
#include "menu.h"

/* Global variables. */
const int FPS				= 60;
const int SCREEN_W			= 800;
const int SCREEN_H			= 800;
const int RESERVE_SAMPLES	= 4;

/* Recive change scene signals. */
SCENE* next_scene = NULL;

/* Input states. */

// Mouse position.
int mouse_x, mouse_y;

// Keyboard state, whether the key is down or not.
ALLEGRO_KEYBOARD_STATE key_state;
// Mouse state, whether the key is down or not.
// 1 is for left, 2 is for right, 3 is for middle.
bool* mouse_state;

/* Game tick for animaiton. */
uint32_t		GAME_TICK			= 0;
uint32_t		SLOWER_GAME_TICK	= 0;
const uint32_t	GAME_TICK_CD		= 64                                                                                            ;
const uint32_t	SLOWER_GAME_TICK_CD = 256;

/* Implementation of GameWindows. */

GAME_WINDOW::GAME_WINDOW() {
	game_title	= "Beat Blast";
	display		= NULL;
	flip_timer	= NULL;
	tick_timer	= NULL;
	event_queue = NULL;
	current_scene = MENU::create(); // First scene will be menu.
	game_done = false;	
}

GAME_WINDOW::~GAME_WINDOW() {
	shared_destroy();
	al_destroy_timer(tick_timer);
	al_destroy_timer(flip_timer);
	al_destroy_event_queue(event_queue);
	al_destroy_display(display);
	free(mouse_state);
	if (current_scene) current_scene->~SCENE();
}

void GAME_WINDOW::game_init() {
	// Set random seed for better random outcome.
	srand(time(NULL));

	allegro5_init();
	game_log("Allegro5 initialized");
	game_log("Game begin");

	// Initialize shared variables.
	shared_init();
	game_log("Game initialized");
	
	// First scene
	game_change_scene(MENU::create());

	// Draw the first frame.
	al_clear_to_color(al_map_rgb(0, 0, 0));
	current_scene->draw();
	al_flip_display();
	game_log("All initialization complete.");
}

void GAME_WINDOW::allegro5_init() {
	if (!al_init())
		game_abort("failed to initialize allegro");

	// Initialize add-ons.
	if (!al_init_primitives_addon())
		game_abort("failed to initialize primitives add-on");
	if (!al_init_font_addon())
		game_abort("failed to initialize font add-on");
	if (!al_init_ttf_addon())
		game_abort("failed to initialize ttf add-on");
	if (!al_init_image_addon())
		game_abort("failed to initialize image add-on");
	if (!al_install_audio())
		game_abort("failed to initialize audio add-on");
	if (!al_init_acodec_addon())
		game_abort("failed to initialize audio codec add-on");
	if (!al_reserve_samples(RESERVE_SAMPLES))
		game_abort("failed to reserve samples");
	if (!al_install_keyboard())
		game_abort("failed to install keyboard");
	if (!al_install_mouse())
		game_abort("failed to install mouse");
	// TODO: Initialize other addons such as video, ...

	// Setup game display.
	display = al_create_display(SCREEN_W, SCREEN_H);
	if (!display)
		game_abort("failed to create display");
	al_set_window_title(display, game_title);

	// Setup update timer.
	flip_timer = al_create_timer(1.0f / FPS);
	if (!flip_timer)
		game_abort("failed to create timer");

	tick_timer = al_create_timer(1.0f / GAME_TICK_CD / 2);
	if (!tick_timer)
		game_abort("faild to create tick timer");

	// Setup event queue.
	event_queue = al_create_event_queue();
	if (!event_queue)
		game_abort("failed to create event queue");

	// Malloc mouse buttons state according to button counts.
	const unsigned m_buttons = al_get_mouse_num_buttons();
	game_log("There are total %u supported mouse buttons", m_buttons);
	// mouse_state[0] will not be used.
	mouse_state = new bool[m_buttons + 1];
	if (mouse_state != NULL) {
		memset(mouse_state, false, (m_buttons + 1) * sizeof(bool));
	}

	// Register display, timer, tick timer, keyboard, mouse events to the event queue.
	al_register_event_source(event_queue, al_get_display_event_source(display));
	al_register_event_source(event_queue, al_get_timer_event_source(tick_timer));
	al_register_event_source(event_queue, al_get_timer_event_source(flip_timer));
	al_register_event_source(event_queue, al_get_keyboard_event_source());
	al_register_event_source(event_queue, al_get_mouse_event_source());
	// TODO: Register other event sources such as timer, video, ...

	// Start the timer to update and draw the game.
	al_start_timer(flip_timer);
}

void GAME_WINDOW::game_start() {
	game_init();

	// This call blocks until the game is finished.
	event_loop();

	// After escape the loop, game ends.
	game_log("Game end");
}

void GAME_WINDOW::event_loop() {
	ALLEGRO_EVENT event;

	int re_draws = 0;
	srand(time(NULL));
	
	while (!game_done) {
		al_wait_for_event(event_queue, &event);
		if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
			// Event for clicking the window close button.
			game_log("Window close button clicked");
			game_done = true;
		}
		else if (event.type == ALLEGRO_EVENT_TIMER) {
			// Event for redrawing the display.
			if (event.timer.source == flip_timer)
			{
				// The redraw timer has ticked.
				re_draws++;
			}
			else if (event.timer.source == tick_timer) {
				// The game tick has ticked.
				GAME_TICK++;
				if (GAME_TICK >= GAME_TICK_CD) {
					GAME_TICK = 0;
				}
				current_scene->update();
			}
		}
		else if (event.type == ALLEGRO_EVENT_KEY_DOWN) {
			// Event for keyboard key down.
			game_log("Key with keycode %d down", event.keyboard.keycode);
			if (event.keyboard.keycode == ALLEGRO_KEY_ESCAPE && current_scene->name == "MENU") {
				game_log("Escape clicked");
				game_done = true;
				continue;
			}
			al_get_keyboard_state(&key_state);
			current_scene->on_key_down();
		}
		else if (event.type == ALLEGRO_EVENT_KEY_UP) {
			// Event for keyboard key up.
			game_log("Key with keycode %d up", event.keyboard.keycode);
			al_get_keyboard_state(&key_state);
			current_scene->on_key_down();
		}
		else if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
			// Event for mouse key down.
			game_log("Mouse button %d down at (%d, %d)", event.mouse.button, event.mouse.x, event.mouse.y);
			mouse_state[event.mouse.button] = true;
			current_scene->on_mouse_down();
		}
		else if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP) {
			// Event for mouse key up.
			game_log("Mouse button %d up at (%d, %d)", event.mouse.button, event.mouse.x, event.mouse.y);
			mouse_state[event.mouse.button] = false;
			current_scene->on_mouse_up();
		}
		else if (event.type == ALLEGRO_EVENT_MOUSE_AXES) {
			if (event.mouse.dx != 0 || event.mouse.dy != 0) {
				// Event for mouse move_script.
				game_log("Mouse move_script to (%d, %d)", event.mouse.x, event.mouse.y);
				mouse_x = event.mouse.x;
				mouse_y = event.mouse.y;
				current_scene->on_mouse_move();
			}
			else if (event.mouse.dz != 0) {
				// Event for mouse scroll.
				game_log("Mouse scroll at (%d, %d) with delta %d", event.mouse.x, event.mouse.y, event.mouse.dz);
				current_scene->on_mouse_scroll();
			}
		}

		// Redraw
		if (re_draws > 0 && al_is_event_queue_empty(event_queue)) {
			if (re_draws > 1)
				game_log("%d frame(s) dropped", re_draws - 1);
			// Update and draw the next frame.
			al_clear_to_color(al_map_rgb(0, 0, 0));
			current_scene->draw();
			al_flip_display();
			re_draws = 0;
		}

		// Change scene
		if (next_scene) {
			game_change_scene(next_scene);
			next_scene = NULL;
		}
	}
}

void GAME_WINDOW::game_change_scene(SCENE* next_scene) {
	game_log("Change scene from %s to %s",
	current_scene->name ? current_scene->name : "(unnamed)",
	next_scene->name ? next_scene->name : "(unnamed)");
	current_scene->~SCENE();
	if(tick_timer == NULL)
		game_abort("NULL game tick timer!!!");
	al_stop_timer(tick_timer);
	current_scene = next_scene;
	if (tick_timer == NULL)
		game_abort("NULL game tick timer!!!");
	al_set_timer_count(tick_timer, 0);
	al_start_timer(tick_timer);
}