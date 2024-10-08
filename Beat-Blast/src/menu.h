#ifndef MENU_H
#define MENU_H
#include "map.h"
#include "scene.h"
#include "utility.h"
#include "shared.h"
#include "normal_stage.h"
#include "game_window.h"
#include "button.h"
#include "player.h"

class MENU : public SCENE {
public:
	MENU();
	~MENU();
	void update(void) override;				// Update MENU scene
	void draw(void) override;				// Draw MENU scene
	void on_key_change(void) override;		// Activate when key change
    void on_mouse_down(void) override;		// Activate when mouse down
	void on_mouse_move(void) override;		// Activate when mouse move
	void on_mouse_up(void) override;		// Activate when mouse up
	void on_mouse_scroll(void) override;	// Activate when mouse scroll
private:
	float draw_x, draw_y;
	Map* map;
	bool damping;
	int dampCNT;
	int dampThreshold;
	vector<BUTTON*>buttons;
	BUTTON* button_start;
	BUTTON* button_oi;
	BUTTON* button_level;
};
#endif