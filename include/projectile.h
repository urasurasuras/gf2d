#ifndef _PROJECTILE_H_
#define _PROJECTILE_H_

#include "entity.h"
#include "player.h"

typedef struct Projectile_S
{
    Player      *ownerPlayer;       /**<Pointer to owner (type Player) of this projectile*/
    Vector2D    direction;          /**<Direction based on controller axis input*/

}Projectile;

Projectile *projectile_new(Player *owner);

Entity *projectile_new_ent(Player *owner, float default_speed, char sprite_path[], Vector2D init_pos);

void projectile_think(Entity *self);

#endif