#ifndef ITEM_H
#define ITEM_H
#include "utility.h"
#include "map.h"

extern const uint32_t GAME_TICK_CD;

class ITEM : OBJECT {
public:
	ITEM(float x, float y, float w, float h, int effect);
	~ITEM();
	void draw() override;
	void update() override;		// Check the validity of next state and update
    RECTANGLE rec;
	float x; float y;
	float w; float h;
private:
	ALLEGRO_COLOR color;		// The background color of ITEM
    int effect;
};								

#endif