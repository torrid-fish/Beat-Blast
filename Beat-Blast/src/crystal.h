#ifndef CRYSTAL_H
#define CRYSTAL_H
#include "utility.h"
#include "map.h"

extern const uint32_t GAME_TICK_CD;

class CRYSTAL : OBJECT {
public:
	CRYSTAL(int grid_x, int grid_y, int element);
	~CRYSTAL();
	void draw() override;
	void update() override;		// Check the validity of next state and update

	RECTANGLE drawrec;			// The actual rectangle that CRYSTAL in
	int grid_x, grid_y;			// The point on the grid map
	int grid_w, grid_h;			// Width and Height on the grid map
	int beatCNT;
private:
	ALLEGRO_COLOR color;		// The background color of CRYSTAL
    // TODO: these should be sync with the music
    // effecting should be 1 when certain sthreshold > beatCNT > 0, vice versa
    bool effecting;             // Whether the player with the same element will be effected at the time 
	int element;                // The element power the CRYSTAL have
	ALLEGRO_BITMAP* sprite;
};								

#endif