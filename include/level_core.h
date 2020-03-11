#ifndef _LEVEL_CORE_H_
#define _LEVEL_CORE_H_
#include "entity.h"
// #include "level.h"

typedef struct Level_core_S
{
    Sprite  *sprite;    /**<Pointer to the sprite of the core*/
    Vector2D    position;
    Vector2D    drawOffset;
    float   health;     /**<Health of core*/
    int     radius;     /**<Radius of core*/
    /* data */
}Level_core;

/**
 * @brief Spawn new core in the middle of the map
 * @param sprite pointer to core sprite
 * */
Level_core *level_core_new(Sprite *sprite);

void level_core_think();

void level_core_touch();

void level_core_draw();

#endif