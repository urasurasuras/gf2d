#include "scene.h"

static int last_pause = 0;
static Scene scene = { 0 };

Scene* scene_get_active() {
	return &scene;
}

void scene_new() {
	//scene = (Scene *)malloc(sizeof(Scene));

}

void scene_purge() {
	//scene = memset(scene, ;
}

void scene_update() {
	if (keys[SDL_SCANCODE_ESCAPE] && last_pause + 256 < SDL_GetTicks()) {
		if (scene.menu_manager->type == MENU_PAUSE) {
			last_pause = SDL_GetTicks();
			switch (scene.menu_manager->_inuse)
			{
			case 0:
				scene.menu_manager->_inuse = 1;
				break;
			case 1:
				scene.menu_manager->_inuse = 0;
				break;
			default:
				break;
			}
		}
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
