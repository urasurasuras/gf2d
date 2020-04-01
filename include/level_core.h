#ifndef _LEVEL_CORE_H_
#define _LEVEL_CORE_H_
#include "entity.h"
// #include "level.h"

/**
 * @brief Spawn new core in the middle of the map
 * @param sprite pointer to core sprite
 * */
Entity *level_core_new(Sprite *sprite, int team);

void level_core_think(Entity *self);

void level_core_touch(Entity *self, Entity *other);

// void level_core_draw();

#endif