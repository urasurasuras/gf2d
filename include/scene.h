#ifndef _SCENE_H_
#define _SCENE_H_

#include "menu.h"
#include "game.h"

#define scn_MENU 1
#define scn_LEVEL 2

Sound* main_theme;
typedef struct {
	int type;
	MenuManager* menu_manager;
	Level* level;
	void* data;

	int paused;
	int done;

	SDL_GameController* c;		/**<Pointer to first game controller, aka. player1*/
}Scene;

Scene* scene_get_active();

void scene_new();

void scene_update();

void scene_draw();

//Toggles scene pause
void scene_pause_toggle();

#endif // !_SCENE_H_
