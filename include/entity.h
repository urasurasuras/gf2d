#ifndef _ENTITY_H_
#define _ENTITY_H_

#include "gf2d_sprite.h"

typedef struct Entity_S
{
    Uint8       _inuse;             /**<Check if entity in memory is active or not*/
    Sprite      *sprite;            /**<A pointer to the sprite*/
    float       frame;              /**<Current frame of sripte*/
    float       maxFrames;           /**<Maximum number of frames in a sprite*/
    Vector2D    position;           /**<2D position of entity*/
    float       speed;              /**<Speed of char*/

    void        *typeOfEnt;         /**<Data pointer*/

    void (*think)(struct Entity_S *self);/**<Called when entity is drawn (updated)*/
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
 * @brief Close entity system
 */
void entity_manager_close();

/**
 * @brief free previously allocated entity
 * @param self a pointer to the entity to free
 */
void entity_free(Entity *self);

/**
 * @brief Update frame info of entity
 * @param self a pointer to the entity to update
 */
void entity_update(Entity *self);

/**
 * @brief update every active entity
 */
void entity_update_all();

/**
 * @brief draw every active entity
 */
void entity_draw_all();

#endif