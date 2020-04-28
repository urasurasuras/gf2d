#ifndef _SCENE_H_
#define _SCENE_H_

#include "menu.h"
#include "game.h"

#define scn_MENU 1
#define scn_LEVEL 2

typedef struct {
	int type;
	MenuManager* menu_manager;
	Level* level;
	void* data;

	int paused;
	int done;
}Scene;

Scene* scene_get_active();

void scene_new();

void scene_update();

void scene_draw();

#endif // !_SCENE_H_
