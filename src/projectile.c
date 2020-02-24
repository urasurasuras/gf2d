#include <stdlib.h>
#include "simple_logger.h"
#include "entity.h"
#include <SDL.h>
#include "projectile.h"
#include "player.h"
#include "gfc_vector.h"
#include <math.h>

Projectile *projectile_new(Entity *owner_entity){
    //Initializations of owner player
    Player  *owner_player;
    owner_player = owner_entity->typeOfEnt;
    //Declaration of projectile
    Projectile *projectile;
    projectile = (Projectile * )malloc(sizeof(Projectile));
    //Initialization of projectile
    projectile->owner_entity = owner_entity;
    slog("Direction: %f.%f",owner_player->direction.x, owner_player->direction.y);
    projectile->angle = vector2d_angle(owner_player->direction);
    slog("Angle %f", projectile->angle);
    //TODO: move projectile if player has no direction
    projectile->direction.x = owner_player->direction.x;
    projectile->direction.y = owner_player->direction.y;
    projectile->speed = 10;
    projectile->time_to_live = 120;//TODO: pass from function
    projectile->time_alive = 0;

    return projectile;
}

Entity *projectile_new_ent(Entity *owner_entity, float default_speed, char sprite_path[], Vector2D init_pos){
    Entity *self;
    self = entity_new();
    if (!self)return NULL;
    //Init ent
    strcpy(self->name, "Projectile");
    self->sprite = gf2d_sprite_load_all(
        sprite_path,
        100,
        100,
        1
    );
    self->radius = 20;
    vector2d_set(self-> drawOffset,-50,-50);
    vector2d_copy(self->position, init_pos);  

    self->typeOfEnt = projectile_new(owner_entity);
    self->think = projectile_think;
    return self;
}

void projectile_think(Entity *self){
    Projectile *p = (Projectile *)self->typeOfEnt;
    p->time_alive += 1;
    if (p->time_alive > p->time_to_live){
        if (self->name)
            slog("Freed: %s", self->name);
        entity_free(self);
        return;
    }
    // slog("Direction %f,%f", p->direction.x, p->direction.y);
    self->position.x += p->direction.x * p->speed;
    self->position.y += p->direction.y * p->speed;
    // slog("Pos: %f.%f", self->position.x, self->position.y);
}
