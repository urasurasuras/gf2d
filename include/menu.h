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

#define MENU_MAIN   1
#define MENU_PAUSE  2

TTF_Font* Sans;

SDL_Rect box_lvl_b;
SDL_Rect box_save;
SDL_Rect box_exit;
SDL_Rect box_backToMain;
SDL_Rect box_play;


typedef struct Menu_S
{
    TextLine    name;           /**<Name of this entity (for debugging)*/

    Uint8       _inuse;             /**<Check if entity in memory is active or not*/
    Sprite      *sprite;            /**<A pointer to the sprite*/
    int         collider_shape;     /**<Indicates whether collider is cirlular or rectangular*/
    // float       frame;              /**<Current frame of sripte*/
    // float       maxFrames;          /**<Maximum number of frames in a sprite*/
    Vector2D    position;           /**<2D position of entity*/
    Vector2D    drawOffset;         /**<Offset of collider*/
    SDL_Rect    box;                /**<Bounds of menu*/
    SDL_Texture *Message;

    Uint32      last_click;         /**<Stores the last SDL Tick this button was clicked*/

    void        (*onClick)(struct Menu_S *self);

}Menu;

typedef struct
{
    Uint8       _inuse;

    int         type;       /**<Type of menu (pause, main)*/

    Sprite*     bg;
    Vector4D*   bg_color;   /**<Color value of menu bg color*/

    Uint32      maxMenus;         /**<Maximum number of entities*/
    Menu* menuList;     /**<List of entities*/
    TTF_Font* text;

    //mouse
    int mx, my;
    float mf;
    Sprite* mouse_sprite;
    Vector4D mouse_color;

    int         clickedThisFrame; /**<True of any UI button was clicked this frame, 
                                  (turned on onClick, turned off at the end of frame)
                                  */

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
MenuManager *menu_manager_init(Uint32 maxMenus, Sprite* bg);

/**
 * @brief Close entity system
 */
void menu_manager_close();

/**
 * @brief Free every button that is used
 */
void menu_free_all();

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

void menu_draw(Menu* self);

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

#endif