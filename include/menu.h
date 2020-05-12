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

#define M_CLICKED   1

#define UI_CLDN     500 //Cooldown for UI in SDL Ticks
SDL_Event e;

TTF_Font* Sans;

SDL_Rect box_lvl_b;
SDL_Rect box_save;
SDL_Rect box_exit;
SDL_Rect box_backToMain;
SDL_Rect box_play;

SDL_Rect box_blue_team;
SDL_Rect box_red_team;

typedef struct Button_S
{
    TextLine    name;               /**<Name of this entity (for debugging)*/

    Uint8       _inuse;             /**<Check if entity in memory is active or not*/
    Sprite      *sprite;            /**<A pointer to the sprite*/
    Vector4D    colorShift;         
    int         collider_shape;     /**<Indicates whether collider is cirlular or rectangular*/
    // float       frame;              /**<Current frame of sripte*/
    // float       maxFrames;          /**<Maximum number of frames in a sprite*/
    Vector2D    position;           /**<2D position of button*/
    Vector2D    drawOffset;         /**<Offset of collider*/
    SDL_Rect    box;                /**<Bounds of menu*/
    SDL_Texture *Message;
    int         toggle;             /**<Used for toggle buttons*/
    int hover;                    /**<whether mouse is hovering over this button*/

    void        (*onClick)(struct Button_S *self);

}Button;

typedef struct
{
    Uint8       _inuse;

    int         type;               /**<Type of menu (pause, main)*/

    Sprite*     bg;
    Vector4D*   bg_color;           /**<Color value of menu bg color*/

    Uint32      maxMenus;           /**<Maximum number of buttons*/
    Button*     buttonList;         /**<List of menu items*/
    TTF_Font* text;

    //mouse
    int mx, my;
    float mf;
    Sprite* mouse_sprite;
    Vector4D mouse_color;
    Uint32      last_click;         /**<Stores the last SDL Tick this button was clicked*/


    int         clickedThisFrame;   /**<True of any UI button was clicked this frame, 
                                    (turned on onClick, turned off at the end of frame)
                                    */

}MenuManager;

MenuManager *get_menu_active();
/**
 * @brief get a pointer to a new button
 * @return NULL on out of memory or error, a pointer to a blank button otherwise
 */
Button *button_new();

/**
 * @brief initialize button resource manager
 * @param maxEnts maximum buttons
 * */
MenuManager *menu_manager_init(Uint32 maxMenus, Sprite* bg);

/**
 * @brief Close button system
 */
void menu_manager_close();

/**
 * @brief Free every button that is used
 */
void button_free_all();

/**
 * @brief free previously allocated button
 * @param self a pointer to the button to free
 */
void button_free(Button *menu);

/**
 * @brief Update frame info of button
 * @param self a pointer to the button to update
 */
void button_update(Button *self);

/**
 * @brief update every active button
 */
void button_update_all();

void button_draw(Button* self);

/**
 * @brief draw every active button
 */
void button_draw_all();

/**
 * @brief For each button, check all other buttons for collision
 * */
void menu_touch_check(Button *menu);

Button *button_generic(
    SDL_Rect    box,
    Vector2D    drawOffset,
    Sprite      *sprite,
    void        (*think)(struct Button_S *self),
    TTF_Font* Sans,
    TextLine     text
);

#endif