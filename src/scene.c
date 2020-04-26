#include "scene.h"

Scene *scene_new() {
	Scene *self = (Scene *)malloc(sizeof(Scene));
	self->mouse_sprite = gf2d_sprite_load_all("images/pointer.png", 32, 32, 16);
	self->mouse_color = vector4d(255,255,255,255);
	self->mf = 0;
	return self;
}

void scene_update(Scene *self) {
	if (!self)return;
	if (self->data) {
		slog("updating scene data");
	}
	if (self->level && self->type == scn_LEVEL) {
		level_update(self->level);
	}
	if (self->menu_manager) {
		menu_update_all();

	}
	
	slog("updating scene");
}

void scene_draw(Scene* self) {
	if (!self)return;
	if (self->data) {
		slog("drawing scene data");
	}
	if (self->level && self->type == scn_LEVEL) {
		level_draw(self->level);
	}
	if (self->menu_manager) {
		menu_draw_all();

		// slog("Paused");
		gf2d_sprite_draw(
			self->mouse_sprite,
			vector2d(self->mx, self->my),
			NULL,
			NULL,
			NULL,
			NULL,
			&self->mouse_color,
			(int)self->mf);

		SDL_GetMouseState(&self->mx, &self->my);
		self->mf += 0.1;
		if (self->mf >= 16.0)self->mf = 0;

	}

	slog("drawing scene");
}
