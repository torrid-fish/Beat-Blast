#ifndef SCENE_H
#define SCENE_H
#include <string>
using namespace std;
// Abstract data type
class SCENE {
public:
	virtual void update() = 0;
	virtual void draw() = 0;
	virtual void on_key_down(int keycode) = 0;
	virtual void on_key_up(int keycode) = 0;
	virtual void on_mouse_down(void) = 0;
	virtual void on_mouse_move(void) = 0;
	virtual void on_mouse_up(void) = 0;
	virtual void on_mouse_scroll(void) = 0;
	const char* name;
};
#endif