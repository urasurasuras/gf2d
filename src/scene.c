#include "scene.h"

static Scene *scene = { 0 };

Scene* scene_get_active() {
	return scene;
}

Scene *scene_new() {
	scene = (Scene *)malloc(sizeof(Scene));
	scene->mouse_sprite = gf2d_sprite_load_all("images/pointer.png", 32, 32, 16);
	scene->mouse_color = vector4d(255,255,255,255);
	scene->mf = 0;
	return scene;
}

void scene_update(Scene *scene) {
	if (!scene)return;
	if (scene->data) {
		slog("updating scene data");
	}
	if (scene->level && scene->type == scn_LEVEL) {
		level_update(scene->level);
	}
	if (scene->menu_manager) {
		menu_update_all();

	}
	
	slog("updating scene");
}

void scene_draw(Scene* scene) {
	if (!scene)return;
	if (scene->data) {
		slog("drawing scene data");
	}
	
	if (scene->level && scene->type == scn_LEVEL) {
		level_draw(scene->level);
	}
	if (scene->menu_manager) {
		menu_draw_all();

		// slog("Paused");
		gf2d_sprite_draw(
			scene->mouse_sprite,
			vector2d(scene->mx, scene->my),
			NULL,
			NULL,
			NULL,
			NULL,
			&scene->mouse_color,
			(int)scene->mf);

		SDL_GetMouseState(&scene->mx, &scene->my);
		scene->mf += 0.1;
		if (scene->mf >= 16.0)scene->mf = 0;

	}

	slog("drawing scene");
}
