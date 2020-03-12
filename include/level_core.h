#ifndef _LEVEL_CORE_H_
#define _LEVEL_CORE_H_
#include "entity.h"
// #include "level.h"

typedef struct Level_core_S
{
    // Sprite  *sprite;    /**<Pointer to the sprite of the core*/
    // Vector2D    position;
    // Vector2D    drawOffset;
    float   health;     /**<Health of core*/
    // int     radius;     /**<Radius of core*/
    /* data */
}Level_core;

/**
 * @brief Spawn new core in the middle of the map
 * @param sprite pointer to core sprite
 * */
Entity *level_core_new(Sprite *sprite, int team);

void level_core_think(Entity *self);

void level_core_touch(Entity *self, Entity *other);

// void level_core_draw();

#endif