#ifndef _ACTIONS_H_
#define _ACTIONS_H_
#include "entity.h"

/**
 * @brief Move the entity in the NORTH direction
 * @param self Pointer to entity
 */
void move_up(Entity *self);

/**
 * @brief Move the entity in the SOUTH direction
 * @param self Pointer to entity
 */
void move_down(Entity *self);

/**
 * @brief Move the entity in the WEST direction
 * @param self Pointer to entity
 */
void move_left(Entity *self);

/**
 * @brief Move the entity in the EAST direction
 * @param self Pointer to entity
 */
void move_right(Entity *self);

/**
 * @brief Respawn entity to 0,0
 * @param self Pointer to entity
 */
void respawn(Entity *self);

#endif
