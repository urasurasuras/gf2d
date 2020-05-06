#ifndef _MENU_EDITOR_H_
#define _MENU_EDITOR_H_
#include "menu.h"
#include "scene.h"
#include "gfc_vector.h"

void menu_editor_init();

void ed_spawn_pickup_health();

Entity* ed_pickup_new(
    TextWord    name,
    Sprite* sprite,
    Vector2D    position,
    void        (*think)(struct Entity_S* self)
);

void ed_move_pickup(Entity* self);

SDL_Rect ed_box_exit;
SDL_Rect ed_box_heal;
#endif // !_MENU_PAUSE_H_
