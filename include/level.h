#ifndef __LEVEL_H__
#define __LEVEL_H__

#include "entity.h"
#include "projectile.h"
#include "level_core.h"

#define LEVEL_WIDTH 1200
#define LEVEL_HEIGHT 720
#define LEVEL_SPAWN_OFFSET 100

#define LEVEL_T_NORMAL 1
#define LEVEL_T_LAVA   2

SDL_Rect bounds_level;
SDL_Rect bounds_stage_wide;
SDL_Rect bounds_stage_narrow;

Vector4D v4d_red;
Vector4D v4d_green;
Vector4D v4d_blue;

typedef struct
{
    Sprite *background;     /**<Background sprite*/
    SDL_Rect bounds_level;        /**<Bounds*/
    SDL_Rect bounds_stage;
    int     frame;          /**<Frame ++ on update*/
    int     done;           /**<Done condition*/
    int     paused;
    int     level_type;
    SJson   *config;
    SJson   *save;
    int     num_pickups;    /**<Number of active pickups in the level*/
    Entity  *core_A;        /**<Pointer to the level core for team A*/
    Entity  *core_B;        /**<Pointer to the level core for team B*/
}Level;

/**
 * @brief Creates level with bg and bounds
 * @param backgroundFile Path to background file
 * @param bounds Level bounds
 * @return Object type of Level
 * */
Level *level_new(char *backgroundFile, SDL_Rect bounds_level, SDL_Rect bounds_stage, int type);

/**
 * @brief Frees level and its sprite
 * */
void level_free(Level *level);

/**
 * @brief Draw level bg image and bounds
 * */
void level_draw(Level *level);

void level_pickups_spawn();

/**
 * @brief Get current level object
 * @return Static type Level
 * */
Level *level_get_active();

/**
 * @brief test if a circle is touching or exceeding the level bounds
 * @return 1 if anything exceeds level bounds
 */
Uint8 level_bounds_test_circle(SDL_Rect bounds, Vector2D center, float radius);

#endif