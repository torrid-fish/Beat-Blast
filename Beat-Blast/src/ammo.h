#ifndef AMMO_H
#define AMMO_H
#include "utility.h"
#include "map.h"

extern const uint32_t GAME_TICK_CD;

typedef enum PATTERN {
	LINEAR = 0,
	SINWAVE = 1,
	FIREBALL = 2
} PATTERN;

class AMMO : OBJECT {
public:
	AMMO(float mid_x, float mid_y, float angle, PATTERN pattern);
	~AMMO();
	static AMMO* create(float mid_x, float mid_y, float angle, PATTERN pattern);	// (x, y): the fix point, angle: the angle relative to horizontal
	void draw() override;
	void update() override;															// Check the validity of next state and update

	RECTANGLE drawrec;																// The rectangle to draw
	ALLEGRO_COLOR color;															// The color of ammo
	int timeCNT;
	PATTERN pattern;
private:
	float angle;
	float fix_x, fix_y;
	ALLEGRO_BITMAP* sprites[3];
};


pair<float, float> LINEAR_tragetory(int t);


 pair<float, float> SINWAVE_tragetory(int t);

#endif