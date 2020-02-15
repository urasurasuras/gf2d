#include "simple_logger.h"
#include "gf2d_draw.h"
#include "level.h"


static Level *current_level = NULL;

Level *level_get_active()
{
    return current_level;
}

Level *level_new(char *backgroundFile, SDL_Rect bounds)
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
    current_level = level;
    slog("Level created %d x %d", level->bounds.w, level->bounds.h);
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
}

Uint8 level_bounds_test_circle(Level *level, Vector2D center, float radius)
{
    Uint8 hit = 0;
    if (!level)
    {
        slog("no level provided for test");
        return 0;
    }
    //slog("Level sprite %c", level->background);
    if (center.x - radius < level->bounds.x)
    {
        hit = 1;
        slog("hiy");
    }
    if (center.y - radius < level->bounds.y)
    {
        hit = 1;
    }
    if (center.x + radius > level->bounds.x + level->bounds.w)
    {
        hit = 1;
    }
    if (center.y - radius > level->bounds.y + level->bounds.h)
    {
        hit = 1;
    }
    
    return hit;
}
