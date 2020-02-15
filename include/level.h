#ifndef __LEVEL_H__
#define __LEVEL_H__

#include "gfc_types.h"
#include "gf2d_sprite.h"


typedef struct
{
    Sprite *background;
    SDL_Rect bounds;
}Level;


Level *level_new();
void level_free(Level *level);
void level_draw(Level *level);
Level *level_get_active();

/**
 * @brief test if a circle is touching or exceeding the level bounds
 */
Uint8 level_bounds_test_circle(Level *level, Vector2D center, float radius, Vector2D *normal);

#endif