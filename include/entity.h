#ifndef _ENTITY_H_
#define _ENTITY_H_

#include "gf2d_sprite.h"

typedef struct Entity_S
{
    Uint8       _inuse;             /**<check if entity in memory is active or not*/
    Sprite      *sprite;            /**<a pointer to the sprite*/
    float       frame;
    Vector2D    position;

    void (*think)(struct Entity_S *self);/**<called when entity is drawn*/
}Entity;
/**
 * @brief get a pointer to a new entity
 * @return NULL on out of memory or error, a pointer to a blank entity otherwise
 */
Entity *entity_new();

/**
 * @brief initialize entity resource manager
 * @param maxEnts maximum entities
 * */
void entity_manager_init(Uint32 maxEnts);

/**
 * @brief free previously allocated entity
 * @param self a pointer to the entity to free
 */
void entity_free(Entity *self);

/**
 * @brief update every active entity
 */
void entity_update_all();

/**
 * @brief draw every active entity
 */
void entity_draw_all();

#endif