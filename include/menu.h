#ifndef _MENU_H_
#define _MENU_H_

#include <stdlib.h>
#include "gf2d_sprite.h"
#include "level.h"
#include "collision.h"
#include "gf2d_graphics.h"
#include <SDL_ttf.h>

#define MENU_BUTTON_HALF_WIDTH 165
#define MENU_BUTTON_HALF_HEIGHT 100

TTF_Font* Sans;

SDL_Rect box_lvl_b;
SDL_Rect box_save;
SDL_Rect box_exit;


typedef struct Menu_S
{
    char        name[20];           /**<Name of this entity (for debugging)*/

    Uint8       _inuse;             /**<Check if entity in memory is active or not*/
    Sprite      *sprite;            /**<A pointer to the sprite*/
    int         collider_shape;     /**<Indicates whether collider is cirlular or rectangular*/
    // float       frame;              /**<Current frame of sripte*/
    // float       maxFrames;          /**<Maximum number of frames in a sprite*/
    Vector2D    position;           /**<2D position of entity*/
    Vector2D    drawOffset;         /**<Offset of collider*/
    SDL_Rect    box;                /**<Bounds of menu*/
    SDL_Texture *Message;


    void        (*think)(struct Menu_S *self);

}Menu;

typedef struct
{
    Sprite* bg;

    Uint32      maxMenus;         /**<Maximum number of entities*/
    Menu* menuList;     /**<List of entities*/
    TTF_Font* text;
}MenuManager;

MenuManager *get_menu_active();
/**
 * @brief get a pointer to a new entity
 * @return NULL on out of memory or error, a pointer to a blank entity otherwise
 */
Menu *menu_new();

/**
 * @brief initialize entity resource manager
 * @param maxEnts maximum entities
 * */
void menu_manager_init(Uint32 maxMenus, Sprite* bg);
/**
 * @brief Close entity system
 */
void menu_manager_close();

/**
 * @brief free previously allocated entity
 * @param self a pointer to the entity to free
 */
void menu_free(Menu *menu);

/**
 * @brief Update frame info of entity
 * @param self a pointer to the entity to update
 */
void menu_update(Menu *self);

/**
 * @brief update every active entity
 */
void menu_update_all();

/**
 * @brief draw every active entity
 */
void menu_draw_all();

/**
 * @brief For each entity, check all other entities for collision
 * */
void menu_touch_check(Menu *menu);

Menu *menu_generic(
    SDL_Rect    box,
    Vector2D    drawOffset,
    Sprite      *sprite,
    void        (*think)(struct Menu_S *self),
    TTF_Font* Sans,
    TextLine     text
);

void button_exit_think (Menu *self);

void button_save_think (Menu *self);

void button_level_think (Menu *self);

#endif