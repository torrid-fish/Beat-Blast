#ifndef MENU_H
#define MENU_H
#include "map.h"
#include "scene.h"
#include "utility.h"
#include "shared.h"
#include "normal_stage.h"
#include "game_window.h"

class MENU : public SCENE {
public:
	MENU();
	static SCENE* create(void);				// Create a pointer to MENU scene
	void update(void) override;				// Update MENU scene
	void draw(void) override;				// Draw MENU scene
	void on_key_down(void) override;		// Activate when key down
	void on_key_up(void) override;			// Activate when key up
    void on_mouse_down(void) override;		// Activate when mouse down
	void on_mouse_move(void) override;		// Activate when mouse move
	void on_mouse_up(void) override;		// Activate when mouse up
	void on_mouse_scroll(void) override;	// Activate when mouse scroll
private:
	Map* map;
};
#endif