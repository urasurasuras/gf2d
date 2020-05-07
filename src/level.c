// #include "entity.h"
#include "level.h"
// #include "level_core.h"

SJson *config;

SJson *level_load_config(){
    config = sj_load("data/config.json");
    return config;
}

SDL_Rect bounds_level           = {0,0,LEVEL_WIDTH,LEVEL_HEIGHT};
SDL_Rect bounds_stage_wide      = {LEVEL_SPAWN_OFFSET/2, LEVEL_SPAWN_OFFSET/2, LEVEL_WIDTH-LEVEL_SPAWN_OFFSET, LEVEL_HEIGHT-LEVEL_SPAWN_OFFSET};
SDL_Rect bounds_stage_narrow    = {LEVEL_SPAWN_OFFSET, LEVEL_SPAWN_OFFSET, LEVEL_WIDTH-LEVEL_SPAWN_OFFSET*2, LEVEL_HEIGHT-LEVEL_SPAWN_OFFSET*2};

Vector4D v4d_red   = {255, 100, 100, 255};
Vector4D v4d_green = {100, 255, 100, 255};
Vector4D v4d_blue  = {100, 100, 255, 255};
Vector4D v4d_yellow= {255, 255, 100, 255};

static Level *current_level = NULL;

Level *level_get_active()
{
    return current_level;
}
Level* level_blank(Sprite* backgroundFile) {
    Level* level;
    if (!backgroundFile)
    {
        slog("No backgroundFile provided, idiot!");
        return NULL;
    }
    level = (Level*)malloc(sizeof(Level));
    if (!level)return NULL;
    entity_manager_init(32);

    level->background = backgroundFile;

    current_level = level;
    return level;

}
Level *level_new(Sprite *backgroundFile, SDL_Rect bounds_level, SDL_Rect bounds_stage_wide, int type)
{
    Level *level;
    if (!backgroundFile)
    {
        slog("No backgroundFile provided, idiot!");
        return NULL;
    }
    level = (Level*)malloc(sizeof(Level)); 
    if(!level)return NULL;
    entity_manager_init(32);
    level->entity_manager = entity_manager_get_active();
    level->background = backgroundFile;
    gfc_rect_set(level->bounds_level, bounds_level.x, bounds_level.y, bounds_level.w, bounds_level.h);
    gfc_rect_set(level->bounds_stage, bounds_stage_wide.x, bounds_stage_wide.y, bounds_stage_wide.w, bounds_stage_wide.h);
    slog("Level created %d x %d", level->bounds_level.w, level->bounds_level.h);
    level->level_type = type;
    level->config = level_load_config();
    level->core_A = level_core_new(gf2d_sprite_load_image("images/cores/core_generic.png"), TEAM_A);
    level->core_A->think = level_core_think;
    level->core_B = level_core_new(gf2d_sprite_load_image("images/cores/core_generic.png"), TEAM_B);
    level->core_B->think = level_core_think;
    level->neutral_monster = grassGuy_new();

    current_level = level;
    return level;
}

//void level_bound_collision_check() {
//    int i;
//    for (i = 0; i < entity_manager_get_active()->maxEnts; i++)
//    {
//        Entity e;
//        e = entity_manager_get_active()->entityList[i];
//
//        if (!e._inuse)continue;
//        //if (&entity_manager_get_active()->entityList[i] == entity)continue;
//
//        if (e.bound_hit) {
//            e.bound_hit(&e);
//        }
//    }
//}
void level_free(Level *level){
    if (!level)return;
    gf2d_sprite_free(level->background);
    level->_inuse = 0;
    //free(level);
    slog("Level freed %d x %d", level->bounds_level.w, level->bounds_level.h);
}

void level_update(Level* level) {
    if (!level)return;
    if (!level->_inuse)return;
    entity_update_all();
    //slog("updating level");
}

void level_draw(Level *level){
    if (!level)return;
    if (!level->_inuse)return;

    //Bg drawn first
    gf2d_sprite_draw_image(level->background,vector2d(0,0));
    gf2d_draw_rect(level->bounds_level, vector4d(0,255,0,255));
    gf2d_draw_rect(level->bounds_stage, vector4d(0,255,100,255));
    //Entities drawn last
    entity_draw_all();
    // level_core_draw();
}

void level_pickups_spawn(){
    SJson* pickupData = sj_load("data/pickups.save");
    SJson* pickupArray;  //array of obs
    SJson* obj;         //ent obj
    SJson* obj_name;    //ent name
    SJson* obj_posArray;    //vector2d floats

    pickupArray = sj_object_get_value(pickupData, "Pickups");
    slog("%d asdasdasd", sj_array_get_count(pickupArray));
    for (int i = 0; i < sj_array_get_count(pickupArray); i++) {
        obj = sj_array_get_nth(pickupArray, i);

        obj_name = sj_object_get_value(obj, "Name");
        obj_posArray = sj_object_get_value(obj, "Position");

        //Set pos
        SJson   *posX, *posY;
        Vector2D pos;
        posX = sj_array_get_nth(obj_posArray, 0);
        posY = sj_array_get_nth(obj_posArray, 1);
        sj_get_float_value(posX, &pos.x);
        sj_get_float_value(posY, &pos.y);

        if (level_get_active()->num_pickups <= 5) {
            char* name_string = (char*)sj_get_string_value(obj_name);

            if (!strcmp(name_string, "Health")) {
                level_pickup_new(
                    name_string,
                    pickup_health,
                    pos,
                    pickup_health_touch
                );
            }
            else if (!strcmp(name_string, "Boost")) {
                level_pickup_new(
                    name_string,
                    pickup_boost,
                    pos,
                    pickup_boost_touch
                );
            }
            else if (!strcmp(name_string, "Speed")) {
                level_pickup_new(
                    name_string,
                    pickup_speed,
                    pos,
                    pickup_speed_touch
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

int_Vector2D level_bounds_test_circle(SDL_Rect bounds, Vector2D center, float radius)
{
    int_Vector2D hit;
    hit.x = 0;
    hit.y = 0;
    // if (!bounds)
    // {
    //     slog("no level provided for test");
    //     return 0;
    // }
    if (center.x - radius < bounds.x)
    {
        hit.x = BOUND_LEFT;    
        // slog("Hitting left border");
    }
    if (center.y - radius < bounds.y)
    {
        hit.y = BOUND_TOP;
        // slog("Hitting top border");
    }
    if (center.x + radius > bounds.x + bounds.w)
    {
        hit.x = BOUND_RIGHT;
        // slog("Hitting right border");
    }
    if (center.y + radius > bounds.y + bounds.h)
    {
        hit.y = BOUND_BOTTOM;
        // slog("Hitting bottom border");
    }
    return hit;
}
