#include <stdlib.h>
#include "simple_logger.h"
#include "menu.h"
#include <SDL.h>
#include <SDL_ttf.h>
#include "gf2d_draw.h"
#include "collision.h"
#include "level.h"
#include "simple_json.h"
#include "simple_json_value.h"
#include "entity.h"
#include "player.h"


static MenuManager menu_manager = {0};

MenuManager *get_menu_active() {
    return &menu_manager;
}

Menu *menu_new(){
    int i;
    for (i=0; i < menu_manager.maxMenus; i++){
        if(menu_manager.menuList[i]._inuse)continue;
        menu_manager.menuList[i]._inuse = 1;
        // slog("Items in menu list %d", i);
        return &menu_manager.menuList[i];
    }
    slog("out of open menu slots in memory");
    return NULL;
}

MenuManager *menu_manager_init(Uint32 maxMenus, Sprite *bg){
    if (menu_manager.menuList != NULL){
        //TODO: cleanup
    }
    if (!maxMenus){
        slog("cannot init 0 zise menu list");
        return;
    }
    menu_manager.menuList = (Menu * )malloc(sizeof(Menu) * maxMenus);
    if (menu_manager.menuList == NULL){
        slog("failed to allocate %i menus for the menu manager",maxMenus);
    }
    menu_manager.maxMenus = maxMenus;
    menu_manager.bg = bg;
    menu_manager.last_click = 0;
    memset(menu_manager.menuList,0,sizeof(Menu)*maxMenus);
    slog("Menu manager initalized");
    atexit(menu_manager_close);
    return &menu_manager;
}

void menu_manager_close(){
    menu_free_all();
    menu_manager.maxMenus = 0;
    free(menu_manager.menuList);
    menu_manager.menuList =NULL;
    slog("menu manager closed");
}

void menu_free_all()
{
    int i;
    for (i = 0; i < menu_manager.maxMenus; i++) {
        if (menu_manager.menuList[i]._inuse) {
            menu_free(&menu_manager.menuList[i]);
        }
    }
}

void menu_free(Menu *self){
    if (!self)return;
    self->_inuse = 0;

    if (!self->sprite)

    gf2d_sprite_free(self->sprite);
    memset(self,0,sizeof(Menu));
}

void menu_update(Menu *self){
    if (!self)return;
    if (self->_inuse == 0) {
        return;
    }
    
    int mx = get_menu_active()->mx;
    int my = get_menu_active()->my;

    if (collide_menu(self->box, vector2d(mx, my))) {
        self->hover = 1;
        //slog("mouseover %s", self->name);
        if (menu_manager.last_click + UI_CLDN < SDL_GetTicks()) {
            if (SDL_GetMouseState(NULL, NULL) && SDL_BUTTON(SDL_BUTTON_LEFT)) {
                if (self->onClick) {
                    slog("%s clicked", self->name);
                    menu_manager.last_click = SDL_GetTicks();
                    self->onClick(self);
                    menu_manager.clickedThisFrame = 1;
                }
            }
        }
    }
    else if (self->hover) {
        self->hover = 0;
    }
    
    /*if (SDL_GetMouseState(&mx, &my) & SDL_BUTTON(SDL_BUTTON_LEFT) && self->last_click + 500 < SDL_GetTicks()) {
        menu_manager.m_state = M_CLICKED;
    }
    if (SDL_GetMouseState(&mx, &my) & SDL_BUTTON(SDL_BUTTON_LEFT) && self->last_click + 500 < SDL_GetTicks()) {
       
    }*/
    // self->frame = self->frame + 0.1;
    // if (self->frame > self->maxFrames)self->frame=0;
    // if (level_bounds_test_circle(level_get_active(), self->position, self->radius))
    // {
    //     //TODO: Do something is ent hits bounds
    // }
    // entity_collision_check(self);
}

void menu_update_all(){
    int i;
    for (i = 0; i < menu_manager.maxMenus; i++)
    {
        if (!menu_manager.menuList[i]._inuse)continue;
        menu_update(&menu_manager.menuList[i]);
    }

    if (!menu_manager.clickedThisFrame) {
        
    }
    
    else
    {
        menu_manager.clickedThisFrame = 0;
    }
}

void menu_draw(Menu *self){
    if (self == NULL){
        slog("cannot draw, null menu provided");
        return;
    }
    gf2d_sprite_draw(
        self->sprite,
        vector2d(self->position.x + self->drawOffset.x,self->position.y + self->drawOffset.y),
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        1
    );
    if (self->hover)    //draw box collider
        gf2d_draw_rect(self->box, vector4d(255, 0, 255, 255));

    if(SDL_RenderCopy(gf2d_graphics_get_renderer(), self->Message, NULL, &self->box)){ //you put the renderer's name first, the Message, the crop size(you can ignore this if you don't want to dabble with cropping), and the rect which is the size and coordinate of your texture
    slog("rendering %s", self->Message);}

    // slog("Pos %f.%f", self->position.x, self->position.y);
}

void menu_draw_all()
{
    if (!menu_manager._inuse)return;
    gf2d_sprite_draw(
        menu_manager.bg,
        vector2d(0,0),
        NULL,
        NULL,
        NULL,
        NULL,
        menu_manager.bg_color,
        NULL);
    int i;
    for (i = 0;i < menu_manager.maxMenus;i++)
    {
        if (!menu_manager.menuList[i]._inuse)continue;
        menu_draw(&menu_manager.menuList[i]);
    }
    gf2d_sprite_draw(
        menu_manager.mouse_sprite,
        vector2d(menu_manager.mx, menu_manager.my),
        NULL,
        NULL,
        NULL,
        NULL,
        &menu_manager.mouse_color,
        (int)menu_manager.mf);

    SDL_GetMouseState(&menu_manager.mx, &menu_manager.my);
    menu_manager.mf += 0.1;
    if (menu_manager.mf >= 16.0)menu_manager.mf = 0;

}

Menu *menu_generic(
    SDL_Rect    box,
    Vector2D    drawOffset,
    Sprite      *sprite,
    void        (*think)(struct Menu_S *self),
    TTF_Font* Sans,
    TextLine     text
){
    Menu *menu;
    menu = menu_new();
    strcpy(menu->name, text);
    menu->box = box;
    menu->drawOffset = drawOffset;
    menu->sprite = sprite;
    menu->onClick = think;
    menu->_inuse = 1;

    SDL_Color White = {255, 255, 255};  // this is the color in rgb format, maxing out all would give you the color white, and it will be your text's color

    SDL_Surface* surfaceMessage = TTF_RenderText_Solid(Sans, text, White); // as TTF_RenderText_Solid could only be used on SDL_Surface then you have to create the surface first

    SDL_Texture* Message = SDL_CreateTextureFromSurface(gf2d_graphics_get_renderer(), surfaceMessage); //now you can convert it into a texture

    menu->Message = Message;
    return menu;
}

SDL_Rect box_lvl_b = {
        (LEVEL_WIDTH / 2) - MENU_BUTTON_HALF_WIDTH,
        (LEVEL_HEIGHT / 2) - MENU_BUTTON_HALF_HEIGHT,
        MENU_BUTTON_HALF_WIDTH * 2,
        MENU_BUTTON_HALF_HEIGHT
};

SDL_Rect box_save = {
    (LEVEL_WIDTH / 2) - MENU_BUTTON_HALF_WIDTH,
    (LEVEL_HEIGHT / 2) - MENU_BUTTON_HALF_HEIGHT * 2,
    MENU_BUTTON_HALF_WIDTH * 2,
    MENU_BUTTON_HALF_HEIGHT
};
SDL_Rect box_play = {
    (LEVEL_WIDTH / 2) - MENU_BUTTON_HALF_WIDTH,
    (LEVEL_HEIGHT / 2) - MENU_BUTTON_HALF_HEIGHT * 2,
    MENU_BUTTON_HALF_WIDTH * 2,
    MENU_BUTTON_HALF_HEIGHT
};
SDL_Rect box_exit = {
    (LEVEL_WIDTH / 2) - MENU_BUTTON_HALF_WIDTH,
    (LEVEL_HEIGHT / 2) + MENU_BUTTON_HALF_HEIGHT,
    MENU_BUTTON_HALF_WIDTH * 2,
    MENU_BUTTON_HALF_HEIGHT
};
SDL_Rect box_backToMain = {
    (LEVEL_WIDTH / 2) - MENU_BUTTON_HALF_WIDTH,
    (LEVEL_HEIGHT / 2) + MENU_BUTTON_HALF_HEIGHT,
    MENU_BUTTON_HALF_WIDTH * 2,
    MENU_BUTTON_HALF_HEIGHT
};