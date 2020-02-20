#ifndef _PROJECTILE_H_
#define _PROJECTILE_H_

#include "entity.h"
#include "player.h"
#include "projectile.h"

typedef struct Projectile_S
{
    Entity      *owner_entity;       /**<Pointer to owner (type Player) of this projectile*/
    Vector2D    direction;          /**<Direction based on controller axis input*/

}Projectile;

Projectile *projectile_new(Entity *owner_entity);

Entity *projectile_new_ent(Entity *owner_entity, float default_speed, char sprite_path[], Vector2D init_pos);

void projectile_think(Entity *self);

#endif