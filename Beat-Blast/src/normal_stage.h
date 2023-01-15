#ifndef NORMAL_STAGE_H
#define NORMAL_STAGE_H
#include "scene.h"
#include "utility.h"
#include "map.h"
#include "shared.h"
#include "player.h"


class NORMAL_STAGE : public SCENE {
public:
	NORMAL_STAGE();
	static SCENE* create(void);				// Create a pointer to NORMAL_STAGE scene
	void update(void) override;				// Update NORMAL_STAGE scene
	void draw(void) override;				// Draw NORMAL_STAGE scene
	void on_key_down(void) override;		// Activate when key down
	void on_key_up(void) override;			// Activate when key up
    void on_mouse_down(void) override;		// Activate when mouse down
	void on_mouse_move(void) override;		// Activate when mouse move
	void on_mouse_up(void) override;		// Activate when mouse up
	void on_mouse_scroll(void) override;	// Activate when mouse scroll
private:
	Map* map;
	PLAYER* player1;
	PLAYER* player2;
	vector<OBJECT *> objects;
};
#endif