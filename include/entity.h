#ifndef _ENTITY_H_
#define _ENTITY_H_

#include "gf2d_sprite.h"

#define SHAPE_CIRCLE    1
#define SHAPE_RECT      2

typedef struct Entity_S
{
    char        name[20];           /**<Name of this entity (for debugging)*/

    Uint8       _inuse;             /**<Check if entity in memory is active or not*/
    Sprite      *sprite;            /**<A pointer to the sprite*/
    int         collider_shape;     /**<Indicates whether collider is cirlular or rectangular*/
    float       frame;              /**<Current frame of sripte*/
    float       maxFrames;          /**<Maximum number of frames in a sprite*/
    Vector2D    position;           /**<2D position of entity*/
    Vector2D    velocity;           /**<2D velocity of entity*/
    int         radius;             /**<how wide this entity is*/
    Vector2D    drawOffset;         /**<Offset of collider*/
    Vector2D    size;               /**<Size vector*/

    void        *typeOfEnt;         /**<Void pointer to whetever this entity is (needs typecast to that type)*/

    void        (*think)(struct Entity_S *self);/**<Called when entity is drawn (updated)*/
    void        (*touch)(struct Entity_S *self, struct Entity_S *other);   /**<called when an entity touches another entity*/

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

/**
 * @brief For each entity, check all other entities for collision
 * */
void entity_collision_check(Entity *entity);

/**
 * @brief Execute the entity's touch function
 * */
void entity_entity_collide(Entity *e1,Entity *e2);

#endif