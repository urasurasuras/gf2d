#ifndef _PROJECTILE_H_
#define _PROJECTILE_H_

#include "entity.h"
#include "player.h"
#include "projectile.h"

typedef struct Projectile_S
{
    Entity      *owner_entity;  /**<Pointer to owner (type Player) of this projectile*/
    float       angle;          /**<Direction based on controller axis input*/
    float       speed;          /**<Default speed of projectile*/
    Vector2D    direction;      /**<Direction projectile is facing (set by owner_player)*/
    int         time_alive;     /**<Time this projectile has been alive for*/
    int         time_to_live;   /**<Maximum amount of tine this projectile has to live*/

}Projectile;

/**
 * @brief Create a projectile by an entity (not Player)
 * @param owner_entity Pointer to the owner ent of this projectile
 * @return Projectile type later used when creating ent
 * */
Projectile *projectile_new(Entity *owner_entity);

/**
 * @brief Create an entity by calling projectile_new
 * @param owner_entity Pointer to owner ent (passed to projectile_new)
 * @param default_speed Speed multiplier of the entity
 * @param sprite_path File path to sprite
 * @param init_pos Initial position of created entity
 * */
Entity *projectile_new_ent(Entity *owner_entity, float default_speed, char sprite_path[], Vector2D init_pos);

/**
 * @brief Called every frame
 * @param self Pointer to self ent
 * */
void projectile_think(Entity *self);

#endif