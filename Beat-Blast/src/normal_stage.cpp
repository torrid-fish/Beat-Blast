#include "normal_stage.h"

static const bool IS_2P = true;

NORMAL_STAGE::NORMAL_STAGE() {
	name = "NORMAL_STAGE";
	map = create_map("Assets/map.txt");
	// Initialize players
	for (int x = 0; x < map->col_num; x++)
		for (int y = 0; y < map->row_num; y++) {
			if (is_p1(map, x, y)) {
				player1 = new PLAYER(x, y, 2, 2, "p1", map);
				game_log("Spawn P1 at %d %d", x, y);
			}
			else if (is_p2(map, x, y) && IS_2P)
				player2 = new PLAYER(x, y, 2, 2, "p2", map);
		}
}

SCENE* NORMAL_STAGE::create() {
	NORMAL_STAGE* temp = new NORMAL_STAGE();
	return temp;
}

void NORMAL_STAGE::update() {
	player1->update();
	if (IS_2P) player2->update();
}
void NORMAL_STAGE::draw() {
	draw_map(map);
	player1->draw();
	if (IS_2P) player2->draw();
}

void NORMAL_STAGE::on_key_down(void) {
	bool up1, down1, left1, right1;
	up1		= key_down(ALLEGRO_KEY_W);
	down1	= key_down(ALLEGRO_KEY_S);
	left1	= key_down(ALLEGRO_KEY_A);
	right1	= key_down(ALLEGRO_KEY_D);

	switch (up1 * 8 + down1 * 4 + left1 * 2 + right1 * 1) {
		case 0:	 // 0000
			player1->next_facing = NONE;
			break;
		case 1:	 // 0001
			player1->next_facing = RIGHT;
			break;
		case 2:	 // 0010
			player1->next_facing = LEFT;
			break;
		case 3:	 // 0011
			player1->next_facing = LEFT_RIGHT;
			break;
		case 4:  // 0100
			player1->next_facing = DOWN;
			break;
		case 5:	 // 0101
			player1->next_facing = DOWN_RIGHT;
			break;
		case 6:	 // 0110
			player1->next_facing = DOWN_LEFT;
			break;
		case 7:	 // 0111
			player1->next_facing = NONE;
			break;
		case 8:  // 1000
			player1->next_facing = UP;
			break;
		case 9:  // 1001
			player1->next_facing = UP_RIGHT;
			break;
		case 10: // 1010
			player1->next_facing = UP_LEFT;
			break;
		case 11: // 1011
			player1->next_facing = NONE;
			break;
		case 12: // 1100
			player1->next_facing = UP_DOWN;
			break;
		case 13: // 1101
			player1->next_facing = NONE;
			break;
		case 14: // 1110
			player1->next_facing = NONE;
			break;
		case 15: // 1111
			player1->next_facing = NONE;
			break;
		default:
			player1->next_facing = NONE;
			break;
	}

	if (IS_2P) {
		bool up2, down2, left2, right2;
		up2 = key_down(ALLEGRO_KEY_UP);
		down2 = key_down(ALLEGRO_KEY_DOWN);
		left2 = key_down(ALLEGRO_KEY_LEFT);
		right2 = key_down(ALLEGRO_KEY_RIGHT);

		switch (up2 * 8 + down2 * 4 + left2 * 2 + right2 * 1) {
			case 0:	 // 0000
				player2->next_facing = NONE;
				break;
			case 1:	 // 0001
				player2->next_facing = RIGHT;
				break;
			case 2:	 // 0010
				player2->next_facing = LEFT;
				break;
			case 3:	 // 0011
				player2->next_facing = LEFT_RIGHT;
				break;
			case 4:  // 0100
				player2->next_facing = DOWN;
				break;
			case 5:	 // 0101
				player2->next_facing = DOWN_RIGHT;
				break;
			case 6:	 // 0110
				player2->next_facing = DOWN_LEFT;
				break;
			case 7:	 // 0111
				player2->next_facing = NONE;
				break;
			case 8:  // 1000
				player2->next_facing = UP;
				break;
			case 9:  // 1001
				player2->next_facing = UP_RIGHT;
				break;
			case 10: // 1010
				player2->next_facing = UP_LEFT;
				break;
			case 11: // 1011
				player2->next_facing = NONE;
				break;
			case 12: // 1100
				player2->next_facing = UP_DOWN;
				break;
			case 13: // 1101
				player2->next_facing = NONE;
				break;
			case 14: // 1110
				player2->next_facing = NONE;
				break;
			case 15: // 1111
				player2->next_facing = NONE;
				break;
			default:
				break;
		}
	}
}
void NORMAL_STAGE::on_key_up(void) {
}
void NORMAL_STAGE::on_mouse_down(void) {
}
void NORMAL_STAGE::on_mouse_move(void) {
}
void NORMAL_STAGE::on_mouse_up(void) {
}
void NORMAL_STAGE::on_mouse_scroll(void) {
}
// // [HACKATHON 2-0]
// // Just modify the GHOST_NUM to 1
// #define GHOST_NUM 4
// /* global variables*/
// extern const uint32_t GAME_TICK_CD;
// extern uint32_t GAME_TICK;
// extern ALLEGRO_TIMER* game_tick_timer;
// ALLEGRO_TIMER* power_up_timer;
// ALLEGRO_TIMER* super_power_up_timer;
// ALLEGRO_TIMER* glitch_timer;
// ALLEGRO_TIMER* flash_timer;
// ALLEGRO_TIMER* freeze_timer;
// ALLEGRO_BITMAP* magic;
// int game_main_Score = 0;
// bool game_over = false;
// bool won = 0;
// bool freeze = 0;
// extern ALLEGRO_SAMPLE* PACMAN_EAT_GHOST;
// extern ALLEGRO_SAMPLE* powerupMusic;
// /* Internal variables*/
// const int power_up_duration = 10;
// static Pacman* pman;
// Map* basic_map;
// Ghost** ghosts;
// ALLEGRO_SAMPLE_ID PACMAN_EAT_GHOST_ID;
// extern ALLEGRO_SAMPLE_ID PACMAN_POWERUP_ID;
// bool debug_mode = false;
// bool cheat_mode = false;
// extern int basic_speed;

// /* Declare static function prototypes */
// static void init(void);
// static void step(void);
// static void checkItem(void);
// static void status_update(void);
// static void update(void);
// static void draw(void);
// static void printinfo(void);
// static void destroy(void);
// static void on_key_down(int key_code);
// static void on_mouse_down(void);
// static void render_init_screen(void);
// static void draw_hitboxes(void);
// static void draw_freezing_magic(void);

// static void init(void) {
// 	won = 0;
// 	magic = al_load_bitmap("Assets/freeze.png");

// 	game_over = false;
// 	game_main_Score = 0;
// 	// create map
// 	//basic_map = create_map(NULL);
// 	// [TODO]
// 	// Create map from .txt file and design your own map !!
// 	//map select
// 	basic_map = create_map("Assets/map_nthu.txt");
// 	//basic_map = create_map("Assets/map.txt");

// 	if (!basic_map) {
// 		game_abort("error on creating map");
// 	}	
// 	// create pacman
// 	pman = pacman_create();
// 	if (!pman) {
// 		game_abort("error on creating pacamn\n");
// 	}
	
// 	// allocate ghost memory
// 	// [HACKATHON 2-1]done
// 	// TODO: Allocate dynamic memory for ghosts array.
// 	ghosts = (Ghost**)malloc(sizeof(Ghost*) * GHOST_NUM);

// 	if (!ghosts) {
// 		game_log("We haven't create any ghosts!\n");
// 	}
// 	else {
// 		// [HACKATHON 2-2]
// 		// TODO: create a ghost.
// 		// Try to look the definition of ghost_create and figure out what should be placed here.
// 		for (int i = 0; i < GHOST_NUM; i++) {
			
// 			game_log("creating ghost %d\n", i);
// 			ghosts[i] = ghost_create(i);  
// 			if (!ghosts[i])
// 				game_abort("error creating ghost\n");
			
// 		}
// 	}
// 	GAME_TICK = 0;

// 	render_init_screen();
// 	power_up_timer = al_create_timer(1.0f); // 1 tick / sec
// 	if (!power_up_timer)
// 		game_abort("Error on create timer\n");
// 	super_power_up_timer = al_create_timer(1.0f); // 1 tick / sec
// 	if (!super_power_up_timer)
// 		game_abort("Error on create timer\n");
// 	glitch_timer = al_create_timer(1.0f); // 1 tick / sec
// 	if (!glitch_timer)
// 		game_abort("Error on create timer\n");
// 	flash_timer = al_create_timer(0.1f); // 1 tick / sec
// 	if (!flash_timer)
// 		game_abort("Error on create timer\n");
// 	freeze_timer = al_create_timer(0.1f); // 1 tick / sec
// 	if (!freeze_timer)
// 		game_abort("Error on create timer\n");


// 	return ;
// }

// static void step(void) {
// 	if (pman->objData.moveCD > 0)
// 		pman->objData.moveCD -= pman->speed;
// 	for (int i = 0; i < GHOST_NUM; i++) {
// 		// important for movement
// 		if (ghosts[i]->objData.moveCD > 0)
// 			ghosts[i]->objData.moveCD -= ghosts[i]->speed;
// 	}
// }
// static void checkItem(void) {
// 	if (pman->flashing) return;
// 	int Grid_x = pman->objData.Coord.x, Grid_y = pman->objData.Coord.y;
// 	if (Grid_y >= basic_map->row_num - 1 || Grid_y <= 0 || Grid_x >= basic_map->col_num - 1 || Grid_x <= 0)
// 		return;
// 	// [HACKATHON 1-3]
// 	// TODO: check which item you are going to eat and use `pacman_eatItem` to deal with it.
	
// 	switch ((basic_map->map)[Grid_y][Grid_x])
// 	{
// 	case '.':
// 		pacman_eatItem(pman, basic_map->map[Grid_y][Grid_x]);
// 		basic_map->map[Grid_y][Grid_x] = ' ';
// 		game_main_Score += 10;
// 		basic_map->beansCount--;
// 		break;
// 	case 'P':
// 		pacman_eatItem(pman, basic_map->map[Grid_y][Grid_x]);
// 		basic_map->map[Grid_y][Grid_x] = ' ';
// 		break;
// 	case 'S':
// 		pacman_eatItem(pman, basic_map->map[Grid_y][Grid_x]);
// 		basic_map->map[Grid_y][Grid_x] = ' ';
// 		break;
// 	case 'F':
// 		pacman_eatItem(pman, basic_map->map[Grid_y][Grid_x]);
// 		basic_map->map[Grid_y][Grid_x] = ' ';
// 		break;
// 	default:
// 		break;
// 	}
	
// 	// [HACKTHON 1-4]
// 	// erase the item you eat from map
// 	// becareful no erasing the wall block.
	
	
// }
// static void status_update(void) {
	
// 	if (al_get_timer_count(power_up_timer) == power_up_duration) {
// 	    pman->powerUp = 0;
// 		al_stop_timer(power_up_timer);
// 		al_set_timer_count(power_up_timer, 0);
// 		stop_bgm(PACMAN_POWERUP_ID);
// 	}
// 	if (al_get_timer_count(super_power_up_timer) == power_up_duration) {
// 		pman->op = 0;
// 		cheat_mode = 0;
// 		al_stop_timer(super_power_up_timer);
// 		al_set_timer_count(super_power_up_timer, 0);
// 	}
// 	if (al_get_timer_count(glitch_timer) == power_up_duration) {
// 		pman->glitching = 0;
// 		pman->speed = basic_speed;
// 		al_stop_timer(glitch_timer);
// 		al_set_timer_count(glitch_timer, 0);
// 	}
// 	if (al_get_timer_count(flash_timer) == 1) {
// 		pman->flashing = 0;
// 		pman->glitching = 0;
// 		pman->speed = basic_speed;
// 		cheat_mode = 0;
// 		al_stop_timer(flash_timer);
// 		al_set_timer_count(flash_timer, 0);
// 	}
// 	if (al_get_timer_count(freeze_timer) == 28) {
// 		freeze = 0;
// 		if (!pman->powerUp) 
// 			for (int i = 0; i < 4; i++) 
// 				ghosts[i]->frozen = 1,
// 				ghosts[i]->speed = 1;
// 		else 
// 			for (int i = 0; i < 4; i++)
// 				if (ghosts[i]->reborn)
// 					ghosts[i]->frozen = 1,
// 					ghosts[i]->speed = 1;
// 		if (!pman->op) cheat_mode = 0;
// 		if (pman->glitching) pman->speed = basic_speed * 2;
// 		else pman->speed = basic_speed;
// 	}
// 	if (al_get_timer_count(freeze_timer) == 100) {
// 		for (int i = 0; i < 4; i++) 
// 			ghosts[i]->speed = basic_speed * (2 - (pman->powerUp)) / 2,
// 			ghosts[i]->frozen = 0;
// 		al_stop_timer(flash_timer);
// 		al_set_timer_count(flash_timer, 0);
// 	}
	
// 	for (int i = 0; i < GHOST_NUM; i++) {
// 		if (ghosts[i]->status == GO_IN) continue;
// 		// [TODO]
// 		// use `getDrawArea(..., GAME_TICK_CD)` and `RecAreaOverlap(..., GAME_TICK_CD)` functions to detect
// 		// if pacman and ghosts collide with each other.
// 		// And perform corresponding operations.
// 		// [NOTE]
// 		// You should have some branch here if you want to implement power bean mode.
// 		// Uncomment Following Code
// 		else if(RecAreaOverlap(getDrawArea(pman->objData, GAME_TICK_CD), getDrawArea(ghosts[i]->objData, GAME_TICK_CD)))
// 		{
// 			if (ghosts[i]->status == FLEE) {
// 				ghosts[i]->reborn = 1;
// 				PACMAN_EAT_GHOST_ID = play_audio(PACMAN_EAT_GHOST, music_volume);
// 				ghosts[i]->status = GO_IN;
// 				ghosts[i]->frozen = 0;
// 				game_main_Score += 200;
// 			}
// 			else if (!cheat_mode) {
// 				game_log("collide with ghost\n");
// 				al_rest(1.0);
// 				pacman_die();
// 				game_over = true;
// 				break;
// 			}
// 		}
		
		
// 	}
// }

// static void update(void) {
	
// 	if (!basic_map->beansCount) {
// 		won = 1;
// 		game_over = 1;
// 		game_log("won\n");
// 	}
// 	if (game_over) {
// 		/*
// 			[TODO]
// 			start pman->death_anim_counter and schedule a game-over event (e.g change scene to menu) after Pacman's death animation finished
// 			game_change_scene(...);
// 		*/
	
// 		al_start_timer(pman->death_anim_counter);
// 		if (al_get_timer_count(pman->death_anim_counter) == 120) {
// 				al_stop_timer(pman->death_anim_counter);
// 				al_set_timer_count(pman->death_anim_counter, 0);
// 				game_change_scene(scene_gameover_create());
// 		 }

// 		return;
// 	}

// 	step();
// 	checkItem();
// 	status_update();
// 	pacman_move(pman, basic_map);
// 	for (int i = 0; i < GHOST_NUM; i++) 
// 		ghosts[i]->move_script(ghosts[i], basic_map, pman);
// }

// static void draw(void) {

// 	al_clear_to_color(al_map_rgb(0, 0, 0));

	
// 	//	[TODO]
// 	//	Draw scoreboard, something your may need is sprinf();
	
// 	char text[50];
// 	sprintf_s(text, 50, "Score: %d", game_main_Score);
// 	al_draw_text(
// 		menuFont,
// 		al_map_rgb(255, 255, 255),
// 		400, 20,
// 		ALLEGRO_ALIGN_CENTRE,
// 		text
// 	);
	
// 	if (freeze) 
// 		draw_freezing_magic();
	
// 	if (pman->glitching) {
// 		al_draw_text(
// 			menuFont,
// 			al_map_rgb(255, 255, 255),
// 			400, 700,
// 			ALLEGRO_ALIGN_CENTRE,
// 			"PRESS E TO GLITCH FORWARD"
// 		);
// 	}
		
// 	draw_map(basic_map);

// 	pacman_draw(pman);
// 	if (game_over)
// 		return;
// 	// no drawing below when game over
// 	for (int i = 0; i < GHOST_NUM; i++)
// 		ghost_draw(ghosts[i]);
	
// 	//debugging mode
// 	if (debug_mode) {
// 		draw_hitboxes();
// 	}

// }

// static void draw_freezing_magic(void) {
// 	game_log("drew the magic");
// 	/*
// 	#define scale_ratio 16 / 1444
// 	#define to_rotate_circles 0.5
// 	#define final_scale 0.5
// 	#define final_angle (360*to_rotate_circles)
// 	#define final_enlarge_time 3.2
// 	#define final_rotate_time 3.2
// 	*/
// 	float t = al_get_timer_count(freeze_timer);
// 	/*float sa = -2 * final_scale / final_enlarge_time / final_enlarge_time * 100;
// 	float ra = -2 * final_angle / final_rotate_time / final_rotate_time / 100.0;
// 	float sv = -1 * sa * final_enlarge_time;
// 	float rv = -1 * ra * final_rotate_time;
// 	*/
// 	float cx = al_get_bitmap_width(magic) / 2;
// 	float cy = al_get_bitmap_height(magic) / 2;
// 	float dx = pman->objData.Coord.x * 21 + 25 + 10;
// 	float dy = pman->objData.Coord.y * 21 + 50 + 10;
// 	/*
// 	float scale = sv * t + sa * t * t / 2;
// 	float an/* = rv * t + ra * t * t / 2*//* = t / final_rotate_time / 10 * final_angle;
// 	float tempan = t / final_rotate_time / 20 * final_angle;
	
// 	if (t <= final_enlarge_time*10) {
// 		al_draw_scaled_rotated_bitmap(
// 			magic,
// 			cx, cy, dx, dy,
// 			scale / scale_ratio, scale / scale_ratio,
// 			an,
// 			0
// 		);
// 	}
// 	else if (t <= final_rotate_time) {
// 		game_log("it's rotating");
// 		al_draw_scaled_rotated_bitmap(
// 			magic,
// 			cx, cy, dx, dy,
// 			final_scale / scale_ratio, final_scale / scale_ratio,
// 			45,
// 			0
// 		);
// 	}
// 	*/
// 	if (t <= 10) {
// 		al_draw_scaled_rotated_bitmap(
// 			magic,
// 			cx, cy, dx, dy,
// 			t / 1444 * 16, t / 1444 * 16,
// 			18 * t,
// 			0
// 		);
// 	}
// 	else if (t<= 25) {
// 		al_draw_scaled_rotated_bitmap(
// 			magic,
// 			cx, cy, dx, dy,
// 			(t / 2 + 5) / 1444*16, (t / 2 + 5) / 1444*16,
// 			t/15 +3,
// 			0
// 		);
// 	}
// 	else {
// 		al_draw_scaled_rotated_bitmap(
// 			magic,
// 			cx, cy, dx, dy,
// 			(25 / 2 + 5) / 1444 * 16, (25 / 2 + 5) / 1444 * 16,
// 			45,
// 			0
// 		);
// 	}
// }

// static void draw_hitboxes(void) {
// 	RecArea pmanHB = getDrawArea(pman->objData, GAME_TICK_CD);
// 	al_draw_rectangle(
// 		pmanHB.x, pmanHB.y,
// 		pmanHB.x + pmanHB.w, pmanHB.y + pmanHB.h,
// 		al_map_rgb_f(1.0, 0.0, 0.0), 2
// 	);

// 	for (int i = 0; i < GHOST_NUM; i++) {
// 		RecArea ghostHB = getDrawArea(ghosts[i]->objData, GAME_TICK_CD);
// 		al_draw_rectangle(
// 			ghostHB.x, ghostHB.y,
// 			ghostHB.x + ghostHB.w, ghostHB.y + ghostHB.h,
// 			al_map_rgb_f(1.0, 0.0, 0.0), 2
// 		);
// 	}

// }

// static void printinfo(void) {
// 	game_log("pacman:\n");
// 	game_log("coord: %d, %d\n", pman->objData.Coord.x, pman->objData.Coord.y);
// 	game_log("PreMove: %d\n", pman->objData.preMove);
// 	game_log("NextTryMove: %d\n", pman->objData.nextTryMove);
// 	game_log("Speed: %f\n", pman->speed);
// }


// static void destroy(void) {
// 	/*
// 		[TODO]
// 		free map array, Pacman and ghosts
// 	*/
// 	freeze = 0;
// 	al_set_timer_count(freeze_timer, 0);
// 	delete_map(basic_map);
// 	pacman_destory(pman);
// 	for (int i = 0; i<GHOST_NUM; i++) ghost_destory(ghosts[i]);
// }

// static void on_key_down(int key_code) {
// 	switch (key_code)
// 	{
// 		// [HACKATHON 1-1]	
// 		// TODO: Use allegro pre-defined enum ALLEGRO_KEY_<KEYNAME> to controll pacman movement
// 		// we provided you a function `pacman_NextMove` to set the pacman's next move direction.
		
// 		case ALLEGRO_KEY_W:
// 			pacman_NextMove(pman, UP);
// 			break;
// 		case ALLEGRO_KEY_A:
// 			pacman_NextMove(pman, LEFT);
// 			break;
// 		case ALLEGRO_KEY_S:
// 			pacman_NextMove(pman, DOWN);
// 			break;
// 		case ALLEGRO_KEY_D:
// 			pacman_NextMove(pman, RIGHT);
// 			break;
// 		case ALLEGRO_KEY_C:
// 			cheat_mode = !cheat_mode;
// 			if (cheat_mode)
// 				printf("cheat mode on\n");
// 			else 
// 				printf("cheat mode off\n");
// 			break;
// 		case ALLEGRO_KEY_E:
// 			if (pman->glitching) {
// 				pman->flashing = 1;
// 				pman->speed = basic_speed * 16;
// 				cheat_mode = 1;
// 				al_stop_timer(flash_timer);
// 				al_set_timer_count(flash_timer, 0);
// 				al_start_timer(flash_timer);
// 				TELEPORT_ID = play_audio(TELEPORT, effect_volume);
// 			}
// 			break;
// 	default:
// 		break;
// 	}

// }

// static void on_mouse_down(void) {
// 	// nothing here

// }

// static void render_init_screen(void) {
// 	al_clear_to_color(al_map_rgb(0, 0, 0));

// 	draw_map(basic_map);
// 	pacman_draw(pman);
// 	for (int i = 0; i < GHOST_NUM; i++) {
// 		ghost_draw(ghosts[i]);
// 	}

// 	al_draw_text(
// 		menuFont,
// 		al_map_rgb(255, 255, 0),
// 		400, 400,
// 		ALLEGRO_ALIGN_CENTER,
// 		"READY!"
// 	);

// 	al_flip_display();
// 	al_rest(2.0);

// }
// // Functions without 'static', 'extern' prefixes is just a normal
// // function, they can be accessed by other files using 'extern'.
// // Define your normal function prototypes below.

// // The only function that is shared across files.
// Scene scene_main_create(void) {
// 	Scene scene;
// 	memset(&scene, 0, sizeof(Scene));
// 	scene.name = "Start";
// 	scene.initialize = &init;
// 	scene.update = &update;
// 	scene.draw = &draw;
// 	scene.destroy = &destroy;
// 	scene.on_key_down = &on_key_down;
// 	scene.on_mouse_down = &on_mouse_down;
// 	// TODO: Register more event callback functions such as keyboard, mouse, ...
// 	game_log("Start scene created");
// 	return scene;
// }
