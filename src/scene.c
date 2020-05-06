#include "scene.h"

static int last_pause = 0;
static Scene scene = { 0 };

Scene* scene_get_active() {
	return &scene;
}

void scene_new() {
	//scene = (Scene *)malloc(sizeof(Scene));
	main_theme = gfc_sound_load("audio/njit-theme-song.mp3", .1, 0);
}

void scene_purge() {
	//scene = memset(scene, ;
}

void scene_update() {
	if (keys[SDL_SCANCODE_ESCAPE] && last_pause + 256 < SDL_GetTicks()) {
		scene_pause_toggle();
	}
	if (scene.data) {
		//slog("updating scene data");
	}
	if (scene.level && scene.type == scn_LEVEL) {
		level_update(scene.level);
	}
	if (scene.menu_manager) {
		menu_update_all();

	}
	
	//slog("updating scene");
}

void scene_draw() {
	if (scene.data) {
		slog("drawing scene data");
	}
	
	if (scene.level && scene.type == scn_LEVEL) {
		level_draw(scene.level);
	}
	if (scene.menu_manager) {
		menu_draw_all();

		// slog("Paused");
		
	}

	//slog("drawing scene");
}

void scene_pause_toggle() {
	if (scene.type == scn_LEVEL) {
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
	}

	if (!scene.paused) {
		scene.menu_manager->_inuse = 1;
		scene.level->_inuse = 0;
	}
	else
	{
		scene.menu_manager->_inuse = 0;
		scene.level->_inuse = 1;
	}
}