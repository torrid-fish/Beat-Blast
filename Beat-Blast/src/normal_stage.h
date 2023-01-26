#ifndef NORMAL_STAGE_H
#define NORMAL_STAGE_H
#include "shared.h"
#include "utility.h"
#include "map.h"
#include "scene.h"
#include "player.h"
#include "small_monster.h"
#include "big_monster.h"
#include "crystal.h"
#include "ammo.h"
#include "item.h"
#include "win.h"
#include "constant.h"

typedef enum STATE {
	DISPLAY_PLAYER = 0,
	DISPLAY_MAP = 1,
	DISPLAY_CRYSTAL = 2,
	DISPLAY_SMALL_MONSTER = 3,
	DISPLAY_BIG_MONSTER = 4,
	DISPLAY_START = 5,
	GAME_PROCEEDING = 6
} STATE;

class NORMAL_STAGE : public SCENE {
public:
	NORMAL_STAGE();
	~NORMAL_STAGE();
	void update(void) override;				// Update NORMAL_STAGE scene
	void draw(void) override;				// Draw NORMAL_STAGE scene
	void on_key_change(void) override;		// Activate when key change
    void on_mouse_down(void) override;		// Activate when mouse down
	void on_mouse_move(void) override;		// Activate when mouse move
	void on_mouse_up(void) override;		// Activate when mouse up
	void on_mouse_scroll(void) override;	// Activate when mouse scroll
private:
	STATE state;							// Game state
	int counter;							// Counter for counting how many update
	Map* map;
	PLAYER* player1;
	PLAYER* player2;
	vector <BIG_MONSTER*> big_monsters;
	vector <SMALL_MONSTER*> small_monsters;
	vector <CRYSTAL*> crystals;
	vector <AMMO*> ammos;
	vector <ITEM*> items;
	bool damping;
	int dampCNT;
	int beatCNT;
	int dampThreshold;
	ALLEGRO_TIMER* death_timer1;
	ALLEGRO_TIMER* death_timer2;
	ALLEGRO_TIMER* beat_timer;
};
#endif