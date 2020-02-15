#include "simple_logger.h"
#include "gf2d_draw.h"
#include "level.h"


static Level *THE_LEVEL = NULL;

Level *level_get_active()
{
    return THE_LEVEL;
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
    level->background = gf2d_sprite_load_image(backgroundFile);
    gfc_rect_set(level->bounds, bounds.x, bounds.y, bounds.w, bounds.h);
    return level;
}

void level_free(Level *level){
    if (!level)return;
    gf2d_sprite_free(level->background);
    free(level);
}

void level_draw(Level *level){
    if (!level)return;
    gf2d_sprite_draw_image(level->background,vector2d(0,0));
    gf2d_draw_rect(level->bounds, vector4d(0,255,0,255));
}

