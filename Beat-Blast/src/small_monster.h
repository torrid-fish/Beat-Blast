#ifndef SMALL_MONSTER_H
#define SMALL_MONSTER_H
#include "utility.h"
#include "map.h"

extern const uint32_t GAME_TICK_CD;

class SMALL_MONSTER : OBJECT {
public:
	SMALL_MONSTER(int grid_x, int grid_y, const char* name, Map* map);
	SMALL_MONSTER(const char* _name, Map* map);
	~SMALL_MONSTER();
	void draw() override;
	void update() override;		// Check the validity of next state and update

	RECTANGLE drawrec;			// The actual rectangle that monster in
	Directions next_facing;		// Record revised result for update
	int next_speed;				// Record revised result for update
	int grid_x, grid_y;			// The point on the grid map
	int grid_w, grid_h;			// Width and Height on the grid map
	Map* map;					// The map that monster use
	float hp;					// Health Point
	float mp;					// Magic Point
private:
	ALLEGRO_COLOR color;		// The background color of monster
	int moveCNT;				// The move counter for drawing smooth animation
	int state;					//	
	int element;                // The element power the monster have
	ALLEGRO_BITMAP* sprite;
};

#endif