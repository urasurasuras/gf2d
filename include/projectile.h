#ifndef _PROJECTILE_H_
#define _PROJECTILE_H_

#include "entity.h"
#include "player.h"
#include "projectile.h"

Sprite *fireball;
Sprite *healing_aura;
Sprite *damage_aura;

typedef struct Projectile_S
{
    Entity      *owner_entity;  /**<Pointer to owner (type Player) of this projectile*/
    float       angle;          /**<Direction based on controller axis input*/
    float       speed;          /**<Default speed of projectile*/
    Vector2D    direction;      /**<Direction projectile is facing (set by owner_player)*/
    int         time_alive;     /**<Time this projectile has been alive for*/
    int         time_to_live;   /**<Maximum amount of tine this projectile has to live*/
    float       strength;        /**<Power level of a projectile*/

}Projectile;

typedef struct Hitscan_S
{
    Entity      *owner_entity;
    Vector2D    p1;
    Vector2D    p2;
    Vector2D    direction;
    float       strenght;
}Hitscan;

/**
 * @brief Pre-loads projectile sprites into individual pointers for later use
 * */
void projectile_load_sprites();

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
    int collider_shape,
    int radius,
    Vector2D draw_offset,
    float strength,
    float speed,
    Vector2D init_pos,
    void (*think)(struct Entity_S *self),
    void (*touch)(struct Entity_S *self, struct Entity_S *other)
);

Entity *hitscan_generic(
    Entity *owner_entity,
    TextWord name,
    int collider_shape,
    float strength//,
    // void (*think)(struct Entity_S *self),
    // void (*touch)(struct Entity_S *self, struct Entity_S *other)
);

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
 * @brief Called every frame
 * @param self Pointer to self ent
 * */
void fireball_think(Entity *self);

void healingAura_think(Entity *self);

void damageAura_think(Entity *self);

void fireball_touch(Entity *self, Entity *other);

void healingAura_touch(Entity *self, Entity *other);

void damageAura_touch(Entity *self, Entity *other);

#endif