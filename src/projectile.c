#include <stdlib.h>
#include "simple_logger.h"
#include "entity.h"
#include <SDL.h>
#include "projectile.h"
#include "player.h"

Projectile *projectile_new(Player *owner){
    Projectile *projectile;
    projectile = (Projectile * )malloc(sizeof(Projectile));
    
    projectile->ownerPlayer = owner;
    projectile->direction = owner->direction;

    return projectile;
}

Entity *projectile_new_ent(Player *owner, float default_speed, char sprite_path[], Vector2D init_pos){
    Entity *self;
    self = entity_new();
    if (!self)return NULL;
    self->sprite = gf2d_sprite_load_all(
        sprite_path,
        100,
        100,
        1
    );
    self->typeOfEnt = projectile_new(owner);
    self->think = projectile_think;
    return self;
}

void projectile_think(Entity *self){
    Projectile *p = (Projectile *)self->typeOfEnt;
    self->position.x += 1;
    self->position.y += 1;
    slog("Pos: %f.%f", self->position.x, self->position.y);
}
