#include "scene.h"

Scene *scene_new() {
	Scene *self = (Scene *)malloc(sizeof(Scene));
	return &self;
}

void scene_update() {
	slog("updating scene");
}