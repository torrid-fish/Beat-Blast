#include "utility.h"
#include "game_window.h"
#include <math.h>
#include <stdlib.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_audio.h>
const int GridSize = 22;
const int block_width = 21,  block_height = 21;			// the pixel size of a "block"
const int map_offset_x = 25, map_offset_y = 50;			// pixel offset of where to start draw map
float VOLUME = 1.0;
extern const int map_offset_x;
extern const int map_offset_y;
extern const int block_width, block_height;
ALLEGRO_SAMPLE* load_audio(const char* filename) {
	ALLEGRO_SAMPLE* sample = al_load_sample(filename);
	if (!sample)
		game_abort("failed to load audio: %s", filename);
	else
		game_log("loaded audio: %s", filename);
	return sample;
}

ALLEGRO_SAMPLE_ID play_audio(ALLEGRO_SAMPLE* sample, float volume) {
	ALLEGRO_SAMPLE_ID id;
	if (!al_play_sample(sample, volume, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, &id))
		game_abort("failed to play audio (once)");
	return id;
}

ALLEGRO_SAMPLE_ID play_bgm(ALLEGRO_SAMPLE* sample, float volume) {
	ALLEGRO_SAMPLE_ID id;
	if (!al_play_sample(sample, volume, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, &id))
		game_abort("failed to play audio (bgm)");
	return id;
}

void stop_bgm(ALLEGRO_SAMPLE_ID sample) {
	al_stop_sample(&sample);
}

ALLEGRO_FONT* load_font(const char* filename, int size) {
	ALLEGRO_FONT* font = al_load_font(filename, size, 0);
	if (!font)
		game_abort("failed to load font: %s with size %d", filename, size);
	else
		game_log("loaded font: %s with size %d", filename, size);
	return font;
}

ALLEGRO_BITMAP* load_bitmap(const char* filename) {
	ALLEGRO_BITMAP* bmp = al_load_bitmap(filename);
	if (!bmp)
		game_abort("failed to load image: %s", filename);
	else
		game_log("loaded image: %s", filename);
	return bmp;
}

ALLEGRO_BITMAP* load_bitmap_resized(const char* filename, int w, int h) {
	ALLEGRO_BITMAP* loaded_bmp = load_bitmap(filename);
	ALLEGRO_BITMAP* resized_bmp = al_create_bitmap(w, h);
	ALLEGRO_BITMAP* prev_target = al_get_target_bitmap();

	if (!resized_bmp)
		game_abort("failed to create bitmap when creating resized image: %s", filename);
    al_set_target_bitmap(resized_bmp);
    al_draw_scaled_bitmap(loaded_bmp, 0, 0,
    al_get_bitmap_width(loaded_bmp),
    al_get_bitmap_height(loaded_bmp),
    0, 0, w, h, 0);
	al_set_target_bitmap(prev_target);
	al_destroy_bitmap(loaded_bmp);

	game_log("resized image: %s", filename);

	return resized_bmp;
}

bool pt_in_rect(int px, int py, RECTANGLE field) {
	if (field.x <= px && field.x + field.w >= px && field.y <= py && field.y + field.h >= py)return 1;
	else return 0;
}

bool RecOverlap(const RECTANGLE RA,const RECTANGLE RB) {
	float RA_x2 = RA.x + RA.w;
	float RA_y2 = RA.y + RA.h;
	float RB_x2 = RB.x + RB.w;
	float RB_y2 = RB.y + RB.h;
	if ((fmin(RA_x2, RB_x2) > fmax(RA.x, RB.x)) && (fmin(RA_y2, RB_y2) > fmax(RA.y, RB.y)))
		return true;
	return false;
}
RECTANGLE get_rec_by_pt_on_board(int grid_x, int grid_y, int grid_w, int grid_h) {
	float x = map_offset_x + grid_x * block_width;
	float y = map_offset_y + grid_y * block_height;
	float w = block_width * grid_w;
	float h = block_height * grid_h;
	RECTANGLE generator(x, y, w, h);
	return generator;
}

RECTANGLE getDrawArea(RECTANGLE rec, int moveCD, Directions facing, uint32_t TOTAL_TICK) {
	RECTANGLE target = rec;

	switch (facing) {
		case UP:
			target.y -= moveCD * block_width / TOTAL_TICK;
			break;
		case DOWN:
			target.y += moveCD * block_width / TOTAL_TICK;
			break;
		case LEFT:
			target.x -= moveCD * block_width / TOTAL_TICK;
			break;
		case RIGHT:
			target.x += moveCD * block_width / TOTAL_TICK;
			break;
		case UP_DOWN:
			break;
		case LEFT_RIGHT:
			break;
		case UP_LEFT:
			target.y -= moveCD * block_width / TOTAL_TICK;
			target.x -= moveCD * block_width / TOTAL_TICK;
			break;
		case DOWN_LEFT:
			target.y += moveCD * block_width / TOTAL_TICK;
			target.x -= moveCD * block_width / TOTAL_TICK;
			break;
		case DOWN_RIGHT:
			target.y += moveCD * block_width / TOTAL_TICK;
			target.x += moveCD * block_width / TOTAL_TICK;
			break;
		case UP_RIGHT:
			target.y -= moveCD * block_width / TOTAL_TICK;
			target.x += moveCD * block_width / TOTAL_TICK;
			break;
		case NONE:
			break;
		default:
			break;
	}

	return target;	
}

void printRecInfo(const RECTANGLE* RA) {
	game_log("RecArea info: \nx: %f, y: %f, w: %f, h: %f\n",
		RA->x, RA->y, RA->w, RA->h);
}
void printDirection(const Directions a) {
	switch (a)
	{
	case NONE:
		game_log("NONE");
		break;
	case UP:
		game_log("UP");
		break;
	case DOWN:
		game_log("DOWN");
		break;
	case LEFT:
		game_log("LEFT");
		break;
	case RIGHT:
		game_log("RIGHT");
		break;
	default:
		game_log("direction error");
		break;
	}
}

bool is_time_to_move(int speed) {
	return GAME_TICK % (GAME_TICK_CD / speed) == 0;
}

uint32_t generateRandomNumber(uint32_t a, uint32_t b) {
	if (b < a) 
		game_abort("Error in RandomNumber, b is less than a");
	return rand() % (b - a + 1) + a;
}

double generateRandomFloat() {
	return (double)rand() / RAND_MAX;
}

bool bernoulliTrail(double p) {
	if (p >= 1 || p <= 0) 
		game_abort("Error range of p = %lf in BernoulliTrail func\n But p should be between 0.0 and 1.0", p);
	return generateRandomFloat() < p;
}

bool movable(int grid_x, int grid_y, int grid_w, int grid_h, Map* M, Directions facing) {
	switch (facing) {
		case UP:
			grid_y--;
			break;
		case DOWN:
			grid_y++;
			break;
		case LEFT:
			grid_x--;
			break;
		case RIGHT:
			grid_x++;
			break;
		case UP_DOWN:
			break;
		case LEFT_RIGHT:
			break;
		case UP_LEFT:
			grid_y--, grid_x--;
			break;
		case DOWN_LEFT:
			grid_y++, grid_x--;
			break;
		case DOWN_RIGHT:
			grid_y++, grid_x++;
			break;
		case UP_RIGHT:
			grid_y--, grid_x++;
			break;
		default:
			break;
	}
	bool res = true;
	for (int i = 0; i < grid_w; i++)
		for (int j = 0; j < grid_h; j++)
			res &= !is_wall(M, grid_x + i, grid_y + j);
	return res;
}

bool is_wall(Map* M, int index_x, int index_y) {
	game_log("is wall %d %d", index_x, index_y);
	if (index_x < 0 || index_x >= M->col_num || index_y < 0 || index_y >= M->row_num)
		return true;
	return M->map[index_y][index_x] == '#';
}
bool is_exit(Map* M, int index_x, int index_y) {
	if (index_x < 0 || index_x >= M->col_num || index_y < 0 || index_y >= M->row_num)
		return false;
	return M->map[index_y][index_x] == 'E';
}
bool is_boss(Map* M, int index_x, int index_y) {
	if (index_x < 0 || index_x >= M->col_num || index_y < 0 || index_y >= M->row_num)
		return false;
	return M->map[index_y][index_x] == 'B';
}
bool is_mini_monster(Map* M, int index_x, int index_y) {
	if (index_x < 0 || index_x >= M->col_num || index_y < 0 || index_y >= M->row_num)
		return false;
	return M->map[index_y][index_x] == 'm';
}
bool is_mega_monster(Map* M, int index_x, int index_y) {
	if (index_x < 0 || index_x >= M->col_num || index_y < 0 || index_y >= M->row_num)
		return false;
	return M->map[index_y][index_x] == 'M';
}
bool is_crystal(Map* M, int index_x, int index_y) {
	if (index_x < 0 || index_x >= M->col_num || index_y < 0 || index_y >= M->row_num)
		return false;
	return M->map[index_y][index_x] == 'O';
}
bool is_p1(Map* M, int index_x, int index_y) {
	if (index_x < 0 || index_x >= M->col_num || index_y < 0 || index_y >= M->row_num)
		return false;
	return M->map[index_y][index_x] == 'P';
}
bool is_p2(Map* M, int index_x, int index_y) {
	if (index_x < 0 || index_x >= M->col_num || index_y < 0 || index_y >= M->row_num)
		return false;
	return M->map[index_y][index_x] == 'p';
}

bool key_down(int allegro_key) {
	return al_key_down(&key_state, allegro_key);
}

// Log using va_list.
static void game_vlog(const char* format, va_list arg);

void game_abort(const char* format, ...) {
	va_list arg;
	va_start(arg, format);
	game_vlog(format, arg);
	va_end(arg);
	fprintf(stderr, "error occured, exiting after 2 secs");
	// Wait 2 secs before exiting.
	al_rest(2);
	// Force exit program.
	exit(1);
}

void game_log(const char* format, ...) {
#ifdef LOG_ENABLED
	va_list arg;
	va_start(arg, format);
	game_vlog(format, arg);
	va_end(arg);
#endif
}

static void game_vlog(const char* format, va_list arg) {
#ifdef LOG_ENABLED
	static bool clear_file = true;
	vprintf(format, arg);
	printf("\n");
	// Write log to file for later debugging.
	FILE* pFile = fopen("log.txt", clear_file ? "w" : "a");
	if (pFile) {
		vfprintf(pFile, format, arg);
		fprintf(pFile, "\n");
		fclose(pFile);
	}
	clear_file = false;
#endif
}