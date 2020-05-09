#include "scene.h"

static int last_pause = 0;
static Scene scene = { 0 };

Scene* scene_get_active() {
	return &scene;
}

void scene_new() {
	scene.c = SDL_GameControllerOpen(0);
	scene.paused = 1;
	//scene = (Scene *)malloc(sizeof(Scene));
}

void scene_purge() {
	//scene = memset(scene, ;
}

void scene_update() {
	if (keys[SDL_SCANCODE_ESCAPE] || SDL_GameControllerGetButton(scene.c, SDL_CONTROLLER_BUTTON_START)) {
		if (last_pause + UI_CLDN < SDL_GetTicks()) {
			last_pause = SDL_GetTicks();
			slog("escape %d", SDL_GetTicks());

			if (editorMode == 0 && scene.type == scn_LEVEL)
				scene_pause_toggle();
		}
			
	}
	if (scene.data) {
		//slog("updating scene data");
	}
	if (scene.level) {
		level_update(scene.level);
	}
	if (scene.menu_manager) {
		menu_update_all();
	}
}

void scene_draw() {
	if (scene.data) {
		slog("drawing scene data");
	}
	
	if (scene.level) {
		level_draw(scene.level);
	}
	if (scene.menu_manager) {
		menu_draw_all();

		// slog("Paused");
		
	}

	//slog("drawing scene");
}

void scene_pause_toggle() {

	last_pause = SDL_GetTicks();

	switch (scene.paused)
	{
	case 0:
		scene.paused = 1;
		break;
	case 1:
		scene.paused = 0;
		break;
	default:
		slog("How did you even- what ??");
		break;
	}


	if (!scene.paused) {//pause
		scene.menu_manager->_inuse = 1;
		scene.level->_inuse = 0;
		Mix_Pause(scene.soundtrack->defaultChannel);
	}
	else//unpause
	{
		scene.menu_manager->_inuse = 0;
		scene.level->_inuse = 1;
		Mix_Resume(scene.soundtrack->defaultChannel);
	}
}