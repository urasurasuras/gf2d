#ifndef __LEVEL_H__
#define __LEVEL_H__

#include "gfc_types.h"
#include "gf2d_sprite.h"

#define LEVEL_WIDTH 1200
#define LEVEL_HEIGHT 720
#define LEVEL_SPAWN_OFFSET 100

typedef struct
{
    Sprite *background;     /**<Background sprite*/
    SDL_Rect bounds;        /**<Bounds*/
    int     frame;          /**<Frame ++ on update*/
    int     done;           /**<Done condition*/
    int     paused;
}Level;

/**
 * @brief Creates level with bg and bounds
 * @param backgroundFile Path to background file
 * @param bounds Level bounds
 * @return Object type of Level
 * */
Level *level_new(char *backgroundFile, SDL_Rect bounds);

/**
 * @brief Frees level and its sprite
 * */
void level_free(Level *level);

/**
 * @brief Draw level bg image and bounds
 * */
void level_draw(Level *level);

/**
 * @brief Get current level object
 * @return Static type Level
 * */
Level *level_get_active();

/**
 * @brief test if a circle is touching or exceeding the level bounds
 * @return 1 if anything exceeds level bounds
 */
Uint8 level_bounds_test_circle(Level *level, Vector2D center, float radius);

#endif