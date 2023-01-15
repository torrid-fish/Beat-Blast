#ifndef MENU_H
#define MENU_H
#include "scene.h"
#include "utility.h"
#include "map.h"
#include "shared.h"

extern Map* create_map(const char* const); 
extern void draw_map(const Map*);

class MENU : public SCENE {
public:
	MENU();
	static SCENE* create(void);
	void update(void) override;
	void draw(void) override;
	void on_key_down(int keycode) override;
	void on_key_up(int keycode) override;
    void on_mouse_down(void) override;
	void on_mouse_move(void) override;
	void on_mouse_up(void) override;
	void on_mouse_scroll(void) override;
private:
	const Map* map;
};
#endif