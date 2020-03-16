// #include "entity.h"
#include "level.h"
// #include "level_core.h"

SJson *config;

SJson *level_load_config(){
    config = sj_load("data/config.json");
    return config;
}

SDL_Rect bounds_normal  = {0,0,LEVEL_WIDTH,LEVEL_HEIGHT};
SDL_Rect bounds_lava    = {LEVEL_SPAWN_OFFSET, LEVEL_SPAWN_OFFSET, LEVEL_WIDTH-LEVEL_SPAWN_OFFSET*2, LEVEL_HEIGHT-LEVEL_SPAWN_OFFSET*2};

Vector4D v4d_red   = {255, 100, 100, 255};
Vector4D v4d_green = {100, 255, 100, 255};
Vector4D v4d_blue  = {100, 100, 255, 255};

static Level *current_level = NULL;

Level *level_get_active()
{
    return current_level;
}

Level *level_new(char *backgroundFile, SDL_Rect bounds, int type)
{
    Level *level;
    if (!backgroundFile)
    {
        slog("No backgroundFile provided, idiot!");
        return NULL;
    }
    level = (Level*)gfc_allocate_array(sizeof(Level), 1); //allocate array
    if(!level)return NULL;
    level->background = gf2d_sprite_load_image(backgroundFile);
    gfc_rect_set(level->bounds, bounds.x, bounds.y, bounds.w, bounds.h);
    slog("Level created %d x %d", level->bounds.w, level->bounds.h);
    level->level_type = type;
    level->config = level_load_config();
    level->core_A = level_core_new(gf2d_sprite_load_image("images/cores/core_generic.png"), TEAM_A);
    level->core_B = level_core_new(gf2d_sprite_load_image("images/cores/core_generic.png"), TEAM_B);

    current_level = level;
    return level;
}

void level_free(Level *level){
    if (!level)return;
    gf2d_sprite_free(level->background);
    free(level);
    slog("Level freed %d x %d", level->bounds.w, level->bounds.h);
}

void level_draw(Level *level){
    if (!level)return;
    gf2d_sprite_draw_image(level->background,vector2d(0,0));
    gf2d_draw_rect(level->bounds, vector4d(0,255,0,255));
    // level_core_draw();
}

void level_pickups_spawn(){
    SJson *pickup_array_config;
    static int max_pickups = 5;
    SJson *config = level_get_active()->config;
    pickup_array_config = sj_object_get_value(config, "Pickups");
    static int random_number = 1;

    srand((unsigned) level_get_active()->frame * time(NULL));

    random_number = rand() % sj_array_get_count(pickup_array_config);
    float randomX = rand() % LEVEL_WIDTH;
    float randomY = rand() % LEVEL_HEIGHT;

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

Uint8 level_bounds_test_circle(Level *level, Vector2D center, float radius)
{
    Uint8 hit = 0;
    if (!level)
    {
        slog("no level provided for test");
        return 0;
    }
    if (center.x - radius < level->bounds.x)
    {
        hit = 1;    
        // slog("Hitting left border");
    }
    if (center.y - radius < level->bounds.y)
    {
        hit = 1;
        // slog("Hitting top border");
    }
    if (center.x + radius > level->bounds.x + level->bounds.w)
    {
        hit = 1;
        // slog("Hitting right border");
    }
    if (center.y + radius > level->bounds.y + level->bounds.h)
    {
        hit = 1;
        // slog("Hitting bottom border");
    }
    
    return hit;
}
