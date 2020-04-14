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

Level *level_new(Sprite *backgroundFile, SDL_Rect bounds_level, SDL_Rect bounds_stage_wide, int type)
{
    Level *level;
    if (!backgroundFile)
    {
        slog("No backgroundFile provided, idiot!");
        return NULL;
    }
    level = (Level*)gfc_allocate_array(sizeof(Level), 1); //allocate array
    if(!level)return NULL;
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
//    for (i = 0; i < entity_manager_get_active().maxEnts; i++)
//    {
//        Entity e;
//        e = entity_manager_get_active().entityList[i];
//
//        if (!e._inuse)continue;
//        //if (&entity_manager_get_active().entityList[i] == entity)continue;
//
//        if (e.bound_hit) {
//            e.bound_hit(&e);
//        }
//    }
//}
void level_free(Level *level){
    if (!level)return;
    gf2d_sprite_free(level->background);
    free(level);
    slog("Level freed %d x %d", level->bounds_level.w, level->bounds_level.h);
}

void level_draw(Level *level){
    if (!level)return;
    gf2d_sprite_draw_image(level->background,vector2d(0,0));
    gf2d_draw_rect(level->bounds_level, vector4d(0,255,0,255));
    gf2d_draw_rect(level->bounds_stage, vector4d(0,255,100,255));
    // level_core_draw();
}

void level_pickups_spawn(){
    SJson *pickup_array_config;
    static int max_pickups = 5;
    SJson *config = level_get_active()->config;
    pickup_array_config = sj_object_get_value(config, "Pickups");
    static int random_number = 1;

    // srand((unsigned) level_get_active()->frame * time(NULL));

    random_number = rand() % sj_array_get_count(pickup_array_config);
    float randomX = 30 + rand() % LEVEL_WIDTH-30;
    float randomY = 30 + rand() % LEVEL_HEIGHT-30;

    SJson *current_pickup = sj_array_get_nth(pickup_array_config,random_number);
    SJson *pickup_name = sj_object_get_value(current_pickup, "Name");

    // sj_echo(current_pickup);
    if (level_get_active()->num_pickups <= max_pickups){
        char *name_string = (char *)sj_get_string_value(pickup_name);

        if (!strcmp(name_string, "pickup_heal")){
            level_pickup_new(
                name_string,
                pickup_health,
                vector2d(randomX,randomY),
                pickup_health_touch
            );
        }
        else if (!strcmp(name_string, "pickup_boost")){
            level_pickup_new(
                name_string,
                pickup_boost,
                vector2d(randomX,randomY),
                pickup_boost_touch
            );
        }
        else if (!strcmp(name_string, "pickup_speed")){
            level_pickup_new(
                name_string,
                pickup_speed,
                vector2d(randomX,randomY),
                pickup_speed_touch
            );
        }
        else
        {
            slog("no matching spritepath");// code to be executed if n doesn't match any cases
        }
        level_get_active()->num_pickups ++;
    }
}

Uint8 level_bounds_test_circle(SDL_Rect bounds, Vector2D center, float radius)
{
    Uint8 hit = 0;
    // if (!bounds)
    // {
    //     slog("no level provided for test");
    //     return 0;
    // }
    if (center.x - radius < bounds.x)
    {
        hit = 4;    
        // slog("Hitting left border");
    }
    if (center.y - radius < bounds.y)
    {
        hit = 1;
        // slog("Hitting top border");
    }
    if (center.x + radius > bounds.x + bounds.w)
    {
        hit = 2;
        // slog("Hitting right border");
    }
    if (center.y + radius > bounds.y + bounds.h)
    {
        hit = 3;
        // slog("Hitting bottom border");
    }
    return hit;
}
