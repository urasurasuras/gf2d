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

	TextWord bluePlayer;
	TextWord redPlayer;

	Sound* soundtrack;			/**<Pointer to the whatever theme song is playing in that the scene*/
	SDL_GameController* c;		/**<Pointer to first game controller, aka. player1*/
}Scene;

Scene* scene_get_active();

void scene_new();

void scene_update();

void scene_draw();

//Toggles scene pause
void scene_pause_toggle();

#endif // !_SCENE_H_
