#ifndef _SCENE_H_
#define _SCENE_H_

#include "menu.h"

typedef struct {
	MenuManager* menu_manager;
	void* data;
}Scene;

Scene *scene_new();

void scene_update();
#endif // !_SCENE_H_
