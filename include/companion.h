#ifndef _COMPANION_H_
#define _COMPANION_H_

#include "entity.h"
#include "projectile.h"

typedef struct Companion_S
{
    Entity      *owner_entity;  /**<Pointer to owner (type Player) of this projectile*/
    int         time_alive;     /**<Time this companion has been alive for*/
    float       strength;       /**<Power level of a projectile*/
    int         behavior;        /**<Current behavior index for companion*/
}Companion;

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
Entity *companion_generic(
    Entity *owner_entity,
    TextWord name,
    Sprite *sprite,
    int collider_shape,
    int radius_body,
    int radius_range,
    Vector2D draw_offset,
    float strength,
    void (*think)(struct Entity_S *self),
    void (*touch)(struct Entity_S *self, struct Entity_S *other),
    void (*detect)(struct Entity_S *self, struct Entity_S *other)
);

/**
 * @brief Generic think function for entities that follow others
 * @param self pointer to self
 * */
void think_behavior(Entity *self);

void lucioAura_touch(Entity *self, Entity *other);

#endif