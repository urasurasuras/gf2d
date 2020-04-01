#ifndef _PLAYER_H
#define _PLAYER_H
#define DEADZONE 2
#define ANALOG_SCALE 10000

#include <SDL.h>
// #include <SDL_ttf.h>
// #include <stdlib.h>
// #include <math.h>
// #include "simple_logger.h"
// #include "simple_json.h"
// #include "simple_json_value.h"
#include "gfc_vector.h"
// #include "gfc_types.h"
// #include "gf2d_sprite.h"
// #include "gf2d_graphics.h"
// #include "gf2d_draw.h"
#include "level.h"
// #include "collision.h"
#include "projectile.h"
#include "entity.h"
#include "companion.h"

// Vector2D spawn_top_left;
// Vector2D spawn_top_right;
// Vector2D spawn_bottom_left;
// Vector2D spawn_bottom_right;

typedef struct Player_S
{   

    int                 index;              /**<Index of character*/
    SDL_GameController *controller;         /**<Pointer to controller*/
    Vector2D            direction;          /**<Direction based on controller axis input*/
    float               angle;              /**<angle of Direction based on controller axis input*/  
    int                 cldn_skill1;
    int                 last_skill1;
    int                 cldn_skill2;
    int                 last_skill2;
    int                 cldn_skill3;
    int                 last_skill3;

    int                 cldn_action1;
    int                 last_action1;

    int                 deployables;        /**<Number of active deployables the players has*/

    // float               health;    
    float               speed;              /**<Speed multipler*/
    float               strength;           /**<Strength multiplier*/
}Player;

/**
 * @brief Spawn 4 players based on spawn locations
 * and pre-loaded sprites
 * */
void players_spawn();

/**
 * @brief Create object type "Player" which contains controller pointer etc.
 * @param speed Speed multiplier for entity
 * @param contNum Index of assigned controller
 * @return Player object to assign to Entity
 */
Player *player_new(float speed, int contNum);

/**
 * @brief Create player-controlled entity
 * @param char_index Conventional index used for controller index
 * @param default_speed Initial speed multiplier for entity (passed into player params)
 * @param sprite_path File path which leads to spritesheet 
 * @param init_pos Initial position for char to spawn at
 * @return Entity that has pointer to player object
 * */
Entity *player_generic(
    TextWord name,
    int char_index, 
    int collider_shape,
    int radius,
    Vector2D draw_offset,
    float default_speed, 
    Sprite *sprite,
    Vector2D init_pos,
    SDL_GameController *controller,
    void (*think)(struct Entity_S *self),
    void (*touch)(struct Entity_S *self, struct Entity_S *other),
    int cldn_skill1,
    int cldn_skill2,
    int cldn_skill3,
    int team
    );
/**
 * @brief Called every draw update by ent
 * Handles movement from controller input
 * @param self Pointer to entity
 * */
void player_think_1 (Entity *self);

void player_touch(Entity *self,Entity *other);

#endif