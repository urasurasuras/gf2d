#ifndef _BOT_H_
#define _BOT_H_
#include "entity.h"
#include "level.h"
#include "simple_logger.h"

typedef struct 
{
    int cldn_action;
    int last_action;
    int cldn_randomized;
    int last_randomized;
    int cldn_movement;
    int last_movement;
}Bot;

Entity *lavaGuy_new();

Entity *grassGuy_new();

void enemy_think (Entity *self);

void enemy_touch (Entity *self, Entity *other);

void enemy_detect(Entity *self, Entity *other);

void not_enemy_think (Entity *self);

void not_enemy_touch (Entity *self, Entity *other);

void not_enemy_detect(Entity *self, Entity *other);

#endif
