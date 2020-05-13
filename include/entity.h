#ifndef _ENTITY_H_
#define _ENTITY_H_
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
// #include "level.h"
// #include "collision.h"
// #include "player.h"
// #include "projectile.h"

#define SHAPE_CIRCLE    1
#define SHAPE_RECT      2
#define SHAPE_LINE      3

#define ENT_PLAYER      1
#define ENT_PROJECTILE  2
#define ENT_HITSCAN     3
#define ENT_RAYSCAN     4
#define ENT_PICKUP      5
#define ENT_CORE        6
#define ENT_COMPANION   7
#define ENT_NEUTRAL_MONSTER 8

#define TEAM_0          0
#define TEAM_A          1
#define TEAM_B          2

typedef struct Entity_S
{
    TextWord    name;               /**<Name of this entity (for debugging)*/
    Uint8       _inuse;             /**<Check if entity in memory is active or not*/

    //Drawing
    Sprite* sprite;                 /**<A pointer to the sprite*/
    Vector4D* color;                /**<A pointer to the color to be shaded while drawing the sprite*/
    Vector2D flip;

    int         f_current;          /**<first frame of current animation*/
    int         f_end;              /**<last frame of current animation*/
    int         f_last;             /**<Last frame this entity was animated*/

    Vector2D    drawOffset;         /**<Offset of collider*/

    SDL_Rect    ui_box;             /**<Used for text display*/
    int         drawhp;             /**<True if health needs to be drawn, defaults to 0*/

    Vector2D    position;           /**<2D position of entity*/
    Vector2D    velocity;           /**<2D velocity of entity*/
    float       health;             /**<Health*/

   
    //Shape
    int         collider_shape;     /**<Indicates whether collider is cirlular or rectangular*/
    int         radius_body;        /**<Radius of curcular body of entity (used for physical collisions)*/
    int         radius_range;       /**<Range for special entity behaviors*/
    Vector2D    size;               /**<Size vector*/
    Vector3D    rotation;           /**<Rotation for sprite*/

    int         collidable;         /**<If this ent it supposed to have physics collisions, off by default*/

    void        *typeOfEnt;         /**<Void pointer to whetever this entity is (needs typecast to that type)*/
    int         type;
    int         team;
    int         lastHit;   /**<Pointer to the entity this one was last hit by*/

    void        (*think)(struct Entity_S *self);/**<Called when entity is drawn (updated)*/
    void        (*touch)(struct Entity_S *self, struct Entity_S *other);   /**<called when an entity touches another entity*/
    void        (*detect)(struct Entity_S *self, struct Entity_S *other);   /**<called when an entity detects another entity within its range*/
    void        (*bound_hit)(struct Entity_S *self, struct int_Vector2D hit_edges);/**<Called when this entity hit the level bounds*/

    //Editor
    int         pickedUp;           /**<Bool if entity is being clicked on my the mouse (on then mouse down, off when up)*/
}Entity;

typedef struct 
{
    Uint32  maxEnts;         /**<Maximum number of entities*/
    Entity  *entityList;     /**<List of entities*/
    TTF_Font* font;
    SDL_Color font_color;
}EntityManager;

/**
 * @brief Returns a pointer to the entity_manager
 * */
EntityManager *entity_manager_get_active();

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
 * @brief free all previously allocated entities
 */
void entity_free_all();
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