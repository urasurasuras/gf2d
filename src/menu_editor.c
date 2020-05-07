#include "menu_editor.h"
#include "menu_main.h"

void menu_editor_init() {

    //load pickup sprites
    pickup_health = gf2d_sprite_load_image("images/pickups/pickup_health.png");
    pickup_boost = gf2d_sprite_load_image("images/pickups/pickup_boost.png");
    pickup_speed = gf2d_sprite_load_image("images/pickups/pickup_speed.png");

    // //Exit button
    menu_generic(
        ed_box_exit,
        vector2d(-100, -250),
        gf2d_sprite_load_image("images/ui/button.png"),
        onClick_exit,
        Sans,
        "Exit"
    );
    //Save button
    menu_generic(
        ed_box_save,
        vector2d(-100, -250),
        gf2d_sprite_load_image("images/ui/button.png"),
        ed_save,
        Sans,
        "Save"
    );

    menu_generic(
        ed_box_heal,
        vector2d(-100, -250),
        pickup_health,
        ed_spawn_pickup_health,
        Sans,
        "Health"
    );
    //get_menu_active()->type = MENU_MAIN;
    get_menu_active()->_inuse = 1;
    scene_get_active()->menu_manager = get_menu_active();
}

void ed_save() {
    SJson* pickupArray; //array of obs
    SJson* obj;         //ent obj
    SJson* obj_name;    //ent name
    SJson* obj_posArray;    //vector2d floats
    
    pickupArray = sj_array_new();

    for (int i = 0; i < entity_manager_get_active()->maxEnts; i++)
    {
        if (entity_manager_get_active()->entityList[i].type == ENT_PICKUP) {
            Entity current_ent = entity_manager_get_active()->entityList[i];
            obj_name = sj_new_str(current_ent.name);

            obj = sj_object_new();

            obj_posArray = sj_array_new();

            sj_array_append(obj_posArray, sj_new_float(current_ent.position.x));
            sj_array_append(obj_posArray, sj_new_float(current_ent.position.y));

            sj_object_insert(obj, "Name", obj_name);
            sj_object_insert(obj, "Position", obj_posArray);

            sj_array_append(pickupArray, obj);
        }
    }
    sj_echo(pickupArray);
    sj_save(pickupArray, "data/pickups.save");
    sj_free(pickupArray);
}

Entity* ed_pickup_new(
    TextWord    name,
    Sprite* sprite,
    Vector2D    position,
    void        (*think)(struct Entity_S* self)
) {
    Entity* self;
    self = entity_new();
    if (!self)return NULL;
    strcpy(self->name, name);
    self->sprite = sprite;
    self->position = position;
    self->think = think;
    self->type = ENT_PICKUP;
    self->collider_shape = SHAPE_CIRCLE;
    self->radius_body = 30;
    self->drawOffset = vector2d(-30, -30);
    return self;
}

void ed_spawn_pickup_health() {
    ed_pickup_new(
        "Health",
        pickup_health,
        vector2d(0, 0),
        ed_move_pickup
    );
}

void ed_move_pickup(Entity* self) {
    if (!self)return;
    if (self->_inuse == 0) {
        return;
    }

    int mx = get_menu_active()->mx;
    int my = get_menu_active()->my;

    if (collide_circle(self->position, self->radius_body, vector2d(mx, my), 0)) {
        SDL_PollEvent(&e);
        if (e.type == SDL_MOUSEBUTTONDOWN) {
            self->pickedUp = 1;
        }
        else if (e.type == SDL_MOUSEBUTTONUP) {
            self->pickedUp = 0;
        }

        if (self->pickedUp) {
            vector2d_copy(self->position, vector2d(mx, my));
            slog("holding %s on tick %d", self->name, SDL_GetTicks());
        }
    }
}

SDL_Rect ed_box_exit = { 
    0, 
    LEVEL_HEIGHT,
    EDITOR_MENU_HEIGHT*3,
    EDITOR_MENU_HEIGHT
};
SDL_Rect ed_box_save = { 
    LEVEL_WIDTH - (EDITOR_MENU_HEIGHT * 3),
    LEVEL_HEIGHT,
    EDITOR_MENU_HEIGHT*3,
    EDITOR_MENU_HEIGHT
};
SDL_Rect ed_box_heal = { 
    LEVEL_WIDTH/2, 
    LEVEL_HEIGHT,
    EDITOR_MENU_HEIGHT,
    EDITOR_MENU_HEIGHT
};