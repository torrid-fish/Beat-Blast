#ifndef MAP_H
#define MAP_H
#define _CRT_SECURE_NO_WARNING
#include "utility.h"
#define MAX_WALL_NUM_W 100
#define MAX_WALL_NUM_H 100

Map* create_map(const char* const filepath);
void delete_map(Map*);

void draw_map(Map*);

//void draw_block(Map*, RecArea);
Directions shortest_path_direc(Map* M, int startGridx, int startGridy, int endGridx, int endGridy);

#endif
