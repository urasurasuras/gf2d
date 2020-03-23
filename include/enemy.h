#ifndef _ENEMY_H_
#define _ENEMY_H_
#include "entity.h"
#include "level.h"

void enemy_think (Entity *self);

void enemy_touch (Entity *self, Entity *other);

void enemy_detect(Entity *self, Entity *other);

#endif
