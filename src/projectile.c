#include <stdlib.h>
#include "simple_logger.h"
#include "entity.h"
#include <SDL.h>
#include "projectile.h"
#include "player.h"
#include "gfc_vector.h"
#include <math.h>

// Projectile *projectile_new(Entity *owner_entity, float speed){
//     //Initializations of owner player
//     Player  *owner_player;
//     owner_player = owner_entity->typeOfEnt;
//     //Declaration of projectile
//     Projectile *projectile;
//     projectile = (Projectile * )malloc(sizeof(Projectile));
//     //Initialization of projectile
//     projectile->owner_entity = owner_entity;
//     vector2d_copy(projectile->direction, owner_player->direction);
//     projectile->speed = speed;
//     projectile->time_to_live = LEVEL_WIDTH/speed;
//     projectile->time_alive = 0;
//     projectile->strength = 50;  //TODO: no magic number
//     return projectile;
// }

// Entity *projectile_new_ent(
//     Entity *owner_entity, 
//     float speed, 
//     char sprite_path[], 
//     Vector2D init_pos
//     )
//     {
//         Entity *self;
//         self = entity_new();
//         if (!self)return NULL;
//         //Init ent
//         strcpy(self->name, "Projectile");
//         self->sprite = gf2d_sprite_load_image(
//             sprite_path
//         );
//         self->radius = 20;
//         vector2d_set(self-> drawOffset,-25,-25);
//         vector2d_copy(self->position, init_pos);  

//         Player  *owner_player;
//         owner_player = owner_entity->typeOfEnt;
        
//         return self;
// }

void projectile_load_sprites(){
    fireball = gf2d_sprite_load_image("images/projectiles/fireball.png");
    healing = gf2d_sprite_load_image("images/projectiles/healing_aura.png");
}

Entity *projectile_generic(
    Entity *owner_entity,
    TextWord name,
    Sprite *sprite,
    int collider_shape,
    int radius,
    Vector2D draw_offset,
    float strength,
    float speed,
    void (*think)(struct Entity_S *self),
    void (*touch)(struct Entity_S *self, struct Entity_S *other)
    )
    {
        Entity *self;
        self = entity_new();
        if (!self)return NULL;
        //Init ent
        Player  *owner_player;
        owner_player = owner_entity->typeOfEnt;
        strcpy(self->name, name);
        self->sprite = sprite;
        self->collider_shape = collider_shape;
        if (collider_shape == SHAPE_CIRCLE)self->radius = radius;
        self->position = owner_entity->position;
        self->drawOffset = draw_offset;   
        self->radius = radius;     

        //Declaration of projectile
        Projectile *projectile;
        projectile = (Projectile * )malloc(sizeof(Projectile));
        //Initialization of projectile
        projectile->owner_entity = owner_entity;
        vector2d_copy(projectile->direction, owner_player->direction);
        projectile->speed = speed;
        projectile->time_to_live = LEVEL_WIDTH/speed;
        projectile->time_alive = 0;
        projectile->strength = strength;
        self->typeOfEnt = (Projectile *)projectile;
        self->type = ENT_PROJECTILE;
        self->think = think;
        self->touch = touch;
        return self;
    }

Entity *hitscan_generic(
    Entity *owner_entity,
    TextWord name,
    int collider_shape,
    float strength//,
    // void (*think)(struct Entity_S *self),
    // void (*touch)(struct Entity_S *self, struct Entity_S *other)
){
    Entity *self;
        self = entity_new();
        if (!self)return NULL;
        //Init ent
        Player  *owner_player;
        owner_player = owner_entity->typeOfEnt;
        strcpy(self->name, name);
        // self->sprite = sprite;
        self->collider_shape = collider_shape;
        // if (collider_shape == SHAPE_CIRCLE)self->radius = radius;
        self->position = owner_entity->position;

        //Declaration of hitscan
        Hitscan *hitscan;
        hitscan = (Hitscan * )malloc(sizeof(Hitscan));
        //Initialization of hitscan
        hitscan->direction = owner_player->direction;
        hitscan->p1 = self->position;

        // float lvl_hyp;
        // lvl_hyp = LEVEL_HEIGHT*LEVEL_HEIGHT
        // hitscan->p2 = 
}
void fireball_think(Entity *self){
    Projectile *p = (Projectile *)self->typeOfEnt;
    p->time_alive += 1;
    if (p->time_alive > p->time_to_live){
        // if (self->name)slog("Freed: %s", self->name);
        entity_free(self);
        return;
    }
    self->position.x += p->direction.x * p->speed;
    self->position.y += p->direction.y * p->speed;
}

void healingAura_think(Entity *self){
    Projectile *p = (Projectile *)self->typeOfEnt;
    p->time_alive += 1;
    if (p->time_alive > p->time_to_live){
        // if (self->name)slog("Freed: %s", self->name);
        entity_free(self);
        return;
    }
}

void fireball_touch(Entity *self, Entity *other){
    // slog("touch called");
    Projectile *p = (Projectile *)self->typeOfEnt;
    Entity *owner_ent = (Entity *)p->owner_entity;
    // Player *owner_player = (Player *)owner_ent->typeOfEnt;
    Player *other_player = (Player *)other->typeOfEnt;
    Projectile *other_projectile = (Projectile *)other->typeOfEnt;
    if (other != owner_ent){
        if (other->type == ENT_PLAYER){
            other_player->health -= p->strength;
            slog("%s has %f", other->name, other_player->health);
            entity_free(self);
            return;
        }
        else if (other->type == ENT_PROJECTILE){
            slog("hit another projectile");
        }
    }
}

void healingAura_touch(Entity *self, Entity *other){
    if (!self)return;
    Projectile *p = (Projectile *)self->typeOfEnt;
    Entity *owner_ent = (Entity *)p->owner_entity;
    Player *owner_player = (Player *)owner_ent->typeOfEnt;
    Player *other_player = (Player *)other->typeOfEnt;

    if (other == owner_ent){
        owner_player->health += 0.1;
        slog("Healed %s %f", other->name, other_player->health);
    }
}