#ifndef _BOT_H_
#define _BOT_H_
#include "entity.h"
#include "level.h"
#include "simple_logger.h"

typedef struct 
{
    int cldn_action;        /**<Static cooldown amount for action */
    int last_action;        /**<Last time that action was done */
    int cldn_randomized;
    int last_randomized;
    int cldn_movement;
    int last_movement;
}Bot;

/**
 * @brief Creates new neutral monster: lava guy
 * This guy moves around eratically and shoots fireballs to players in its range
 * @return Monster entity
 * */
Entity *lavaGuy_new();

/**
 * @brief Creates new neutral monster: grass guy
 * This guy moves around calmly and shoots healing to players in its range
 * @return Monster entity
 * */
Entity *grassGuy_new();

/**
 * @brief Think function for lava guy
 * @param self self
 * */
void enemy_think (Entity *self);

/**
 * @brief Touch function for lava guy
 * @param self self
 * @param other Other entity that is colliding with self
 * */
void enemy_touch (Entity *self, Entity *other);

/**
 * @brief ranged attack detect function for lava guy
 * @param self self
 * @param other other entity
 * */
void enemy_detect(Entity *self, Entity *other);

/**
 * @brief Think function for grass guy
 * @param self self
 * */
void not_enemy_think (Entity *self);

/**
 * @brief Touch function for grass guy
 * @param self self
 * @param other Other entity that is colliding with self
 * */
void not_enemy_touch (Entity *self, Entity *other);

/**
 * @brief ranged attack detect function for grass guy
 * @param self self
 * @param other other entity
 * */
void not_enemy_detect(Entity *self, Entity *other);

#endif
