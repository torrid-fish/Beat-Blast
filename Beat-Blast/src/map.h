#ifndef MAP_H
#define MAP_H
#define _CRT_SECURE_NO_WARNING
#include "shared.h"
#include "utility.h"

Map* create_map(const char* const filepath);
void delete_map(Map*);
void draw_map(Map*);

/* Declare static function prototypes. */
static void draw_wall_index(Map* M, int row, int col);
static void draw_exit_index(Map* M, int row, int col);
static void draw_board_index(Map* M, const int row, const int col);
static void draw_ground_index(Map* M, const int row, const int col);
#endif
