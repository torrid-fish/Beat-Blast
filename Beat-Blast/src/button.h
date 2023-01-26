#ifndef BUTTON_H
#define BUTTON_H
#include "utility.h"
#include "shared.h"
#include "map.h"

extern const uint32_t GAME_TICK_CD;

class BUTTON : OBJECT {
public:
	BUTTON(float x, float y, float w, float h, const char* name);
	~BUTTON();
	void draw() override;
	void update() override;		
	bool buttonHover(int mouse_x, int mouse_y);
	RECTANGLE rec;			
	bool hovered;
	float draw_x, draw_y;
private:
	// ALLEGRO_BITMAP* default_img;
	// ALLEGRO_BITMAP* hovered_img;
	ALLEGRO_COLOR color;		// The background color of BUTTON
	ALLEGRO_COLOR hovered_color;		// The background color of BUTTON
};								

#endif