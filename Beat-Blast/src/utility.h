#ifndef SCENE_UTILITY_H
#define SCENE_UTILITY_H
#define _CRT_SECURE_NO_DEPRECATE
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_primitives.h>
#include <vector>
#include <iostream>
#include <string>
using namespace std;
/* Constants regarding grid */
extern const int block_width,  block_height;			// the pixel size of a "block"
extern const int map_offset_x, map_offset_y;			// pixel offset of where to start draw map

/* Utility functions */

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

	float x, y;	// left up corner of the RECTANGLE
	float w, h;
	//	----- <- h
	//	|	| <- w
	//	-----
};

// Directions
typedef enum Directions {
	NONE = 0,
	UP = 1,
	LEFT = 2,
	RIGHT = 3,
	DOWN = 4,
	UP_DOWN = 5,
	LEFT_RIGHT = 6,
	UP_LEFT = 7,
	DOWN_LEFT = 8,
	DOWN_RIGHT = 9,
	UP_RIGHT = 10
} Directions;

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
	int beansNum;
	int beansCount;
	char** map;
} Map;

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

// Generate random unsigned 32bit integer
uint32_t generateRandomNumber(uint32_t a, uint32_t b);

// Generate random float number
double generateRandomFloat(void);

// An random strategy
bool bernoulliTrail(double p);

// Whether next block is wall
bool movable(int grid_x, int grid_y, int grid_w, int grid_h, Map* M, Directions facing);

bool is_wall(Map* M, int index_x, int index_y);
bool is_exit(Map* M, int index_x, int index_y);
bool is_mini_monster(Map* M, int index_x, int index_y);
bool is_mega_monster(Map* M, int index_x, int index_y);
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
