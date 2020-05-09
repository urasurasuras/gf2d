#ifndef _PROJECTILE_H_
#define _PROJECTILE_H_

#include <SDL.h>
#include <SDL_ttf.h>
#include <stdlib.h>
#include <math.h>
#include "simple_logger.h"
#include "simple_json.h"
#include "simple_json_value.h"
#include "gfc_vector.h"
#include "gfc_types.h"
#include "gf2d_sprite.h"
#include "gf2d_graphics.h"
#include "gf2d_draw.h"
#include "level.h"
#include "collision.h"
#include "player.h"
#include "entity.h"
#include "PRELOADER.h"

typedef struct Projectile_S
{
    Entity      *owner_entity;  /**<Pointer to owner (type Player) of this projectile*/
    float       angle;          /**<Direction based on controller axis input*/
    float       speed;          /**<Default speed of projectile*/
    int         time_alive;     /**<Time this projectile has been alive for*/
    int         time_to_live;   /**<Maximum amount of tine this projectile has to live*/
    float       strength;       /**<Power level of a projectile*/

    Entity      *firstContact;  /**<Entity that has the first point of contact (mostly intended for hitscan)*/

    int         cldn_1;
    int         last_cldn_1;

}Projectile;

// typedef struct Hitscan_S
// {
//     Entity      *owner_entity;
//     // Vector2D    p1;
//     // Vector2D    p2;
//     Vector2D    direction;
//     float       strenght;
// }Hitscan;

/**
 * @brief Returns a generic projectile with all the necessary members initialized
 * @param owner_entity Entity that this entity is spawned from
 * @param name Name string (for debugging)
 * @param sprite Pointer to preloaded sprite
 * @param collider_shape Cricular or rectangular
 * @param radius Only used if shape is curcilar
 * @param draw_offset Sprite draw offset relative to entity position
 * @param strength Damage/healing power of this projectile that affects other entities
 * @param speed Speed multiplier of this projectile
 * @return Entity into entity_manager
 * */
Entity *projectile_generic(
    Entity *owner_entity,
    TextWord name,
    Sprite *sprite,
    int f_end,
    int collider_shape,
    int radius_body,
    int radius_range,
    float strength,
    float speed,
    int time_to_live,
    Vector2D init_pos,
    void (*think)(struct Entity_S *self),
    void (*touch)(struct Entity_S *self, struct Entity_S *other),
    void (*detect)(struct Entity_S *self, struct Entity_S *other)
);

Entity *hitscan_generic(
    Entity *owner_entity,
    TextWord name,
    int collider_shape,
    float strength,
    int time_to_live,
    float angleOffset,
    void (*think)(struct Entity_S *self),
    void (*touch)(struct Entity_S *self, struct Entity_S *other),
    int type
);
Entity* fireball_projectile(Entity* owner_entity);

// /**
//  * @brief Create a projectile by an entity (not Player)
//  * @param owner_entity Pointer to the owner ent of this projectile
//  * @return Projectile type later used when creating ent
//  * */
// Projectile *projectile_new(Entity *owner_entity, float speed);

// /**
//  * @brief Create an entity by calling projectile_new
//  * @param owner_entity Pointer to owner ent (passed to projectile_new)
//  * @param default_speed Speed multiplier of the entity
//  * @param sprite_path File path to sprite
//  * @param init_pos Initial position of created entity
//  * */
// Entity *projectile_new_ent(Entity *owner_entity, float speed, char sprite_path[], Vector2D init_pos);

/**
 * @brief Generic think function for entites that have 
 * constant velocity inherited from its owner 
 * (used for projectiles etc.)
 * @param self Pointer to self ent
 * @return Then time_to_live is exceeded
 * */
void think_move_constVel(Entity *self);

/**
 * @brief Generic think function for entities that are 
 * stationary (used for turrets, auras etc.)
 * @param self Pointer to self
 * @return Then time_to_live is exceeded
 * */
void think_stationary(Entity *self);

void fireball_think(Entity *self);

void healingAura_think(Entity *self);

void damageAura_think(Entity *self);

void hitscan_think(Entity *self);

void rayscan_think(Entity *self);

void turret_think(Entity *self);

void fireball_touch(Entity *self, Entity *other);

void healingAura_touch(Entity *self, Entity *other);

void damageAura_touch(Entity *self, Entity *other);

void hitscan_touch(Entity *self, Entity *other);

void rayscan_touch(Entity *self, Entity *other);

void turret_touch(Entity *self, Entity *other);

void heal_dart_touch(Entity *self, Entity *other);

void landmine_touch(Entity *self, Entity *other);
void detect_test(Entity *self, Entity *other);

void turret_detect(Entity *self, Entity *other);

Entity *level_pickup_new(
    TextWord    name,
    Sprite      *sprite,
    Vector2D    position,
    void        (*touch)(struct Entity_S *self, struct Entity_S *other)
);

void pickup_health_touch(Entity *self, Entity *other);
void pickup_boost_touch(Entity *self, Entity *other);
void pickup_speed_touch(Entity *self, Entity *other);

#endif