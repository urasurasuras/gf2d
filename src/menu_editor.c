#include "menu_editor.h"
#include "menu_main.h"

void menu_editor_init() {

    //load pickup sprites
    pickup_health = gf2d_sprite_load_image("images/pickups/pickup_health.png");
    pickup_boost = gf2d_sprite_load_image("images/pickups/pickup_boost.png");
    pickup_speed = gf2d_sprite_load_image("images/pickups/pickup_speed.png");

    //Exit button
    button_generic(
        ed_box_exit,
        vector2d(-100, -250),
        NULL,
        onClick_exit,
        Sans,
        "Exit"
    );
    //Delete button (toggle)
    button_generic(
        ed_box_delete,
        vector2d(-100, -250),
        NULL,
        ed_delete,
        Sans,
        "Delete"
    );
    //Save button
    button_generic(
        ed_box_save,
        vector2d(-100, -250),
        NULL,
        ed_save,
        Sans,
        "Save"
    );
    //Load button
    button_generic(
        ed_box_load,
        vector2d(-100, -250),
        NULL,
        ed_load,
        Sans,
        "Load"
    );

    //Pickup buttons
    button_generic(
        ed_box_heal,
        vector2d(-100, -250),
        pickup_health,
        ed_spawn_pickup_health,
        Sans,
        ""
    );
     button_generic(
        ed_box_speed,
        vector2d(-100, -250),
        pickup_speed,
        ed_spawn_pickup_speed,
        Sans,
        ""
    );
     button_generic(
        ed_box_boost,
        vector2d(-100, -250),
        pickup_boost,
        ed_spawn_pickup_boost,
        Sans,
        ""
    );
    //get_menu_active()->type = MENU_MAIN;
    get_menu_active()->_inuse = 1;
    scene_get_active()->menu_manager = get_menu_active();
    get_menu_active()->type = MENU_MAIN;
}

void ed_save() {
    SJson* pickupData = sj_object_new();  //data obj
    SJson* pickupArray = sj_array_new(); //array of obs
    SJson* obj;         //ent obj
    SJson* obj_name;    //ent name
    SJson* obj_posArray;    //vector2d floats
    
    for (int i = 0; i < entity_manager_get_active()->maxEnts; i++)
    {
        slog("%s type %d", entity_manager_get_active()->entityList[i].name,entity_manager_get_active()->entityList[i].type);
        if (entity_manager_get_active()->entityList[i].type == ENT_EDITABLE/* == ENT_PICKUP*/) {
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
    sj_object_insert(pickupData, "Pickups", pickupArray);
    sj_save(pickupData, "data/pickups.save");

    sj_free(pickupData);
}

void ed_load() {
    SJson* pickupData = sj_load("data/pickups.save");
    SJson* pickupArray;  //array of obs
    SJson* obj;         //ent obj
    SJson* obj_name;    //ent name
    SJson* obj_posArray;    //vector2d floats
    entity_free_all();
    pickupArray = sj_object_get_value(pickupData, "Pickups");
    //slog("%d asdasdasd", sj_array_get_count(pickupArray));
    for (int i = 0; i < sj_array_get_count(pickupArray); i++) {
        obj = sj_array_get_nth(pickupArray, i);

        obj_name = sj_object_get_value(obj, "Name");
        obj_posArray = sj_object_get_value(obj, "Position");

        //Set pos
        SJson* posX, * posY;
        Vector2D pos;
        posX = sj_array_get_nth(obj_posArray, 0);
        posY = sj_array_get_nth(obj_posArray, 1);
        sj_get_float_value(posX, &pos.x);
        sj_get_float_value(posY, &pos.y);

        if (level_get_active()->num_pickups <= 5) {
            char* name_string = (char*)sj_get_string_value(obj_name);

            if (!strcmp(name_string, "Health")) {
                ed_pickup_new(
                    name_string,
                    pickup_health,
                    pos,
                    ed_move_pickup
                );
            }
            else if (!strcmp(name_string, "Boost")) {
                ed_pickup_new(
                    name_string,
                    pickup_boost,
                    pos,
                    ed_move_pickup
                );
            }
            else if (!strcmp(name_string, "Speed")) {
                ed_pickup_new(
                    name_string,
                    pickup_speed,
                    pos,
                    ed_move_pickup
                );
            }
            else
            {
                slog("no matching spritepath");// code to be executed if n doesn't match any cases
            }
            level_get_active()->num_pickups++;
        }
    }
}

void ed_delete() {
    //change mouse color
    switch (editorMode)
    {
    case 1:
        editorMode = 2;
        get_menu_active()->mouse_color = vector4d(255, 0, 0, 255);

        /*for (int i = 0; i < entity_manager_get_active()->maxEnts; i++) {
            if (!entity_manager_get_active()->entityList[i]._inuse)continue;
            if (!entity_manager_get_active()->entityList[i].type != ENT_EDITABLE)continue;
            entity_manager_get_active()->entityList[i].think = ed_delete_pickup;
        }*/

        break;
    case 2:
        editorMode = 1;
        get_menu_active()->mouse_color = vector4d(255, 255, 255, 255);

        /*for (int i = 0; i < entity_manager_get_active()->maxEnts; i++) {
            if (!entity_manager_get_active()->entityList[i]._inuse)continue;
            if (!entity_manager_get_active()->entityList[i].type != ENT_EDITABLE)continue;
            entity_manager_get_active()->entityList[i].think = ed_move_pickup;
        }*/

        break;

    default:
        break;
    }
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
    //self->type = ENT_PICKUP;
    self->collider_shape = SHAPE_CIRCLE;
    self->radius_body = 30;
    self->drawOffset = vector2d(-30, -30);
    self->type = ENT_EDITABLE;
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
void ed_spawn_pickup_boost(){
    ed_pickup_new(
        "Boost",
        pickup_boost,
        vector2d(0, 0),
        ed_move_pickup
    );
}
void ed_spawn_pickup_speed(){
    ed_pickup_new(
        "Speed",
        pickup_speed,
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

        if (SDL_GetMouseState(NULL, NULL) && SDL_BUTTON(SDL_BUTTON_LEFT)) {
            if (editorMode == 1) {
                if (!self->pickedUp)
                    self->pickedUp = 1;
                else if (self->pickedUp)
                    self->pickedUp = 0;
            }
            else if (editorMode == 2) {
                entity_free(self);
                return;
            }
        }
    }
    if (self->pickedUp) {
        vector2d_copy(self->position, vector2d(mx, my));
        slog("holding %s on tick %d", self->name, SDL_GetTicks());
    }
}

//void ed_delete_pickup(Entity* self)
//{
//    if (!self)return;
//    if (self->_inuse == 0) {
//        return;
//    }
//
//    int mx = get_menu_active()->mx;
//    int my = get_menu_active()->my;
//
//    if (collide_circle(self->position, self->radius_body, vector2d(mx, my), 0)) {
//        slog("mousing over %s", self->name);
//        if (get_menu_active()->last_click + UI_CLDN < SDL_GetTicks()) {
//            if (SDL_GetMouseState(NULL, NULL) && SDL_BUTTON(SDL_BUTTON_LEFT)) {
//                slog("clicked %s", self->name);
//
//                get_menu_active()->last_click = SDL_GetTicks();
//                self->_inuse = 0;
//            }
//        }
//    }
//}

SDL_Rect ed_box_exit = { 
    LEVEL_WIDTH - (EDITOR_MENU_HEIGHT * 3),
    LEVEL_HEIGHT,
    EDITOR_MENU_HEIGHT*3,
    EDITOR_MENU_HEIGHT
};
SDL_Rect ed_box_delete = { 
    LEVEL_WIDTH - (EDITOR_MENU_HEIGHT * 6),
    LEVEL_HEIGHT,
    EDITOR_MENU_HEIGHT*3,
    EDITOR_MENU_HEIGHT
};
SDL_Rect ed_box_save = { 
    0,
    LEVEL_HEIGHT,
    EDITOR_MENU_HEIGHT*3,
    EDITOR_MENU_HEIGHT
};
SDL_Rect ed_box_load = { 
    EDITOR_MENU_HEIGHT * 3,
    LEVEL_HEIGHT,
    EDITOR_MENU_HEIGHT*3,
    EDITOR_MENU_HEIGHT
};
//Pickup buttons
SDL_Rect ed_box_heal = { 
    LEVEL_WIDTH/2, 
    LEVEL_HEIGHT,
    EDITOR_MENU_HEIGHT,
    EDITOR_MENU_HEIGHT
};
SDL_Rect ed_box_speed = { 
    LEVEL_WIDTH/2 + EDITOR_MENU_HEIGHT, 
    LEVEL_HEIGHT,
    EDITOR_MENU_HEIGHT,
    EDITOR_MENU_HEIGHT
};
SDL_Rect ed_box_boost = { 
    LEVEL_WIDTH / 2 - EDITOR_MENU_HEIGHT,
    LEVEL_HEIGHT,
    EDITOR_MENU_HEIGHT,
    EDITOR_MENU_HEIGHT
};