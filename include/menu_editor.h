#ifndef _MENU_EDITOR_H_
#define _MENU_EDITOR_H_
#include "menu.h"
#include "scene.h"
#include "gfc_vector.h"

#define ENT_EDITABLE    32  //"entity.h"
void menu_editor_init();

void ed_spawn_pickup_health();
void ed_spawn_pickup_boost();
void ed_spawn_pickup_speed();

/**
@brief saves all spawned pickups into a JSON file
*/
void ed_save();

void ed_load();

//Toggles between delete mode and normal mode
void ed_delete();


Entity* ed_pickup_new(
    TextWord    name,
    Sprite* sprite,
    Vector2D    position,
    void        (*think)(struct Entity_S* self)
);

void ed_move_pickup(Entity* self);
//void ed_delete_pickup(Entity* self);


SDL_Rect ed_box_exit;
SDL_Rect ed_box_delete;
SDL_Rect ed_box_save;
SDL_Rect ed_box_load;
SDL_Rect ed_box_heal;
SDL_Rect ed_box_speed;
SDL_Rect ed_box_boost;
#endif // !_MENU_PAUSE_H_
