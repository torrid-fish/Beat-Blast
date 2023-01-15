#ifndef PLAYER_H
#define PLAYER_H
#include "utility.h"
#include <math.h>
#include "map.h"

extern const uint32_t GAME_TICK_CD;

class PLAYER : OBJECT {
public:
	PLAYER(int grid_x, int grid_y, int grid_w, int grid_h, const char* _name, Map* map);
	~PLAYER();
	void draw() override;
	void update() override;		// Check the validity of next state and update

	Directions next_facing;		// Record revised result for update
	int next_speed;				// Record revised result for update
	int grid_x, grid_y;			// The point on the grid map
	int grid_w, grid_h;			// Width and Height on the grid map
	Map* map;					// The map that player use
private:
	int moveCD;					// Count down for drawing smooth movement
	int state;					//
    int hp;						// Health Point
    int	mp;						// Magic Point
	int element;                // The element power the player have
    int combo;                  // The combo counter
};								

#endif