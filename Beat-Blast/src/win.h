#ifndef WIN_H
#define WIN_H
#include "map.h"
#include "scene.h"
#include "utility.h"
#include "shared.h"
#include "normal_stage.h"
#include "game_window.h"
#include "button.h"
#include "menu.h"
class WIN : public SCENE {
public:
	WIN();
	~WIN();
	void update(void) override;				// Update MENU scene
	void draw(void) override;				// Draw MENU scene
	void on_key_change(void) override;		// Activate when key change
	void on_mouse_down(void) override;		// Activate when mouse down
	void on_mouse_move(void) override;		// Activate when mouse move
	void on_mouse_up(void) override;		// Activate when mouse up
	void on_mouse_scroll(void) override;	// Activate when mouse scroll
private:
	int draw_x, draw_y;
	Map* map;
	bool damping;
	int dampCNT;
	int dampThreshold;
	int drawCNT;
};
#endif