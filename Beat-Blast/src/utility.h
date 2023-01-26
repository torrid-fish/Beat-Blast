#ifndef SCENE_UTILITY_H
#define SCENE_UTILITY_H
#define _CRT_SECURE_NO_DEPRECATE

// allegro libraries
#include <allegro5/allegro.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_native_dialog.h>

// c++ libraries
#include <iostream>
#include <algorithm>
#include <iterator>
#include <string>
#include <vector>
#include <queue>
#include <stack>
#include <tuple>
#include <set>
#include <map>
// c libraries
#include <stdlib.h>
#include <math.h>

using namespace std;
using ll = long long;
using pii = pair<int, int>;

/* Constants regarding grid */
extern int block_width,  block_height;			// the pixel size of a "block"
extern int fix_map_offset_x, fix_map_offset_y;	// fix point of offset
extern int map_offset_x, map_offset_y;			// pixel offset of where to start draw map
#define MAX_WALL_NUM_W 100
#define MAX_WALL_NUM_H 100
/* Utility functions */

#define pi acos(-1)

// Load audio and check if failed.
// Supports .wav, .flac, .ogg, .it, .mod, .s3m, .xm.
ALLEGRO_SAMPLE* load_audio(const char* filename);

// Play audio once.
ALLEGRO_SAMPLE_ID play_audio(ALLEGRO_SAMPLE* sample, float volume);

// Play audio loop.
ALLEGRO_SAMPLE_ID play_bgm(ALLEGRO_SAMPLE* sample, float volume);

// Stop audio.
void stop_bgm(ALLEGRO_SAMPLE_ID sample);

// Load font and check if failed.
// Supports .ttf.
ALLEGRO_FONT* load_font(const char* filename, int size);

// Load bitmap and check if failed.
// Supports .bmp, .pcx, .tga, .jpg, .png.
ALLEGRO_BITMAP* load_bitmap(const char* filename);

// Load resized bitmap and check if failed.
ALLEGRO_BITMAP* load_bitmap_resized(const char* filename, int w, int h);

class RECTANGLE {
public:
	RECTANGLE() {
		x = 0; y = 0;
		w = 0; h = 0;
	}
	RECTANGLE(float x, float y, float w, float h) {
		this->x = x; this->y = y; 
		this->w = w; this->h = h;
	}

	float midx() { return x + w / 2; }
	float midy() { return y + h / 2; }

	float x, y;	// top left point of the RECTANGLE
	float w, h; // w goes with x, while h goes with y
};

// Directions
typedef enum Directions {
	NONE 		= 0,
	UP 			= 1,
	LEFT 		= 2,
	RIGHT 		= 3,
	DOWN 		= 4,
	UP_DOWN 	= 5,
	LEFT_RIGHT 	= 6,
	UP_LEFT 	= 7,
	DOWN_LEFT 	= 8,
	DOWN_RIGHT 	= 9,
	UP_RIGHT 	= 10
} Directions;

// temp for shortest path
const int four_probe[4][2] = {{ 1, 0 }, { 0, 1 }, { -1,0 }, { 0, -1 }};

// Abstract data type
class OBJECT {
public:
	OBJECT() {
		layer = 0;
		speed = 0;
		facing = NONE;
		name = "NONE";
	}
	int layer;					// The layer of object (the smaller be drawn earlier)
	int speed;					// speed
	const char* name;			// The name of the object
	RECTANGLE rec;				// The rectangle of the object
	Directions facing;			// Direction of facing
	virtual void draw() = 0;	// Draw the object
	virtual void update() = 0;	// Update the object
};

typedef struct Map{
	int row_num, col_num;
	int wallnum;
	char** map;
} Map;

// Rotate point (x, y) based on (0, 0) with angle (>0) clockwise.
pair<float, float> Rotation2D(float x, float y, float angle);

// Estimation distance function
int ChebyshevDistance(int grid_x_start, int grid_y_start, int grid_x_end, int grid_y_end);

// Return the distance square
int EuclideanDistanceSquare(int grid_x_start, int grid_y_start, int grid_x_end, int grid_y_end);

// Called from generate_A_star_Table to generate result
Directions A_star(Map* M, int grid_w, int grid_h, int grid_x_start, int grid_y_start, int grid_x_end, int grid_y_end);

#define QUEUE_SIZE 600
Directions shortest_path_direc(Map* M, int grid_x_start, int grid_y_start, int grid_x_end, int grid_y_end);

// Check whether point are in rectangle
bool pt_in_rect(int px, int py, RECTANGLE field);

// Check whether two rectangle overlap
bool RecOverlap(const RECTANGLE RA,const RECTANGLE RB);

// Generate the RECTANGLE on the screen by the position on the grid block
RECTANGLE get_rec_by_pt_on_board(int grid_x, int grid_y, int grid_w, int grid_h);

// Calculate the rectangle we are going draw
RECTANGLE getDrawArea(RECTANGLE rec, int moveCD, Directions facing, uint32_t TOTAL_TICK);

// Print the infomation of rectangle
void printRecInfo(const RECTANGLE *RA);

// Print the direction
void printDirection(const Directions a);

// Calculate whether its time to change grid_x, grid_y
bool is_time_to_move(int speed);

// Calculate the position during dampedOscillation
// A0:				Initial maximum shift
// time:			Current time (This function will be X(t))
// angular_freq:	The angular frequency
// alpha:			The constant of b/2m
float dampedOscillation(float A0, float time, float angular_freq, float alpha);

// Generate random unsigned 32bit integer
uint32_t generateRandomNumber(uint32_t a, uint32_t b);

// Generate random direction
Directions generatRandomDirection(void);

// Generate random float number
double generateRandomFloat(void);

pair<int, int> generateRandomSpawnGridPoint(Map* map, int grid_w, int grid_h);

// An random strategy
bool bernoulliTrail(double p);

// Whether next block is valid
bool movable(int grid_x, int grid_y, int grid_w, int grid_h, Map* M, Directions facing);

bool is_wall(Map* M, int index_x, int index_y);
bool is_exit(Map* M, int index_x, int index_y);
bool is_boss(Map* M, int index_x, int index_y);
bool is_outside(Map* M, int index_x, int index_y);
bool is_small_monster(Map* M, int index_x, int index_y);
bool is_big_monster(Map* M, int index_x, int index_y);
bool is_crystal(Map* M, int index_x, int index_y);
bool is_p1(Map* M, int index_x, int index_y);
bool is_p2(Map* M, int index_x, int index_y);

extern ALLEGRO_KEYBOARD_STATE key_state;

bool key_down(int allegro_key);

/* Debug Test */
// Display error message and exit the program, used like 'game_log'.
// Write formatted output to stdout and file from the format string.
// If the program crashes unexpectedly, you can inspect "log.txt" for
// further information.
void game_abort(const char* format, ...);

// Log events for later debugging, used like 'game_log'.
// Write formatted output to stdout and file from the format string.
// You can inspect "log.txt" for logs in the last run.
void game_log(const char* format, ...);

#endif
