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
    memset(menu_manager.menuList,0,sizeof(Menu)*maxMenus);
    slog("Menu manager initalized");
    atexit(menu_manager_close);
    return &menu_manager;
}

void menu_manager_close(){
    int i;
    for (i=0; i < menu_manager.maxMenus; i++){
        if(menu_manager.menuList[i]._inuse){
            menu_free(&menu_manager.menuList[i]);
        }
    }
    menu_manager.maxMenus = 0;
    free(menu_manager.menuList);
    menu_manager.menuList =NULL;
    slog("menu manager closed");
}

void menu_free(Menu *self){
    if (!self)return;
    if (!self->sprite)
    gf2d_sprite_free(self->sprite);
    memset(self,0,sizeof(Menu));
}

void menu_update(Menu *self){
    if (!self)return;
    int mx, my;
    SDL_GetMouseState(&mx, &my);
    if (collide_menu(self->box, vector2d(mx, my))) {
        if (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_LEFT) && self->last_click + 500 < SDL_GetTicks()) {
            self->last_click = SDL_GetTicks();
            if (self->onClick) {
                self->onClick(self);
            }
        }
    }
    
    // self->frame = self->frame + 0.1;
    // if (self->frame > self->maxFrames)self->frame=0;
    // if (level_bounds_test_circle(level_get_active(), self->position, self->radius))
    // {
    //     //TODO: Do something is ent hits bounds
    // }
    // entity_collision_check(self);
}

void menu_update_all(){
    // level_get_active()->frame ++;
    int i;
    for (i = 0;i < menu_manager.maxMenus;i++)
    {
        if (!menu_manager.menuList[i]._inuse)continue;
        menu_update(&menu_manager.menuList[i]);
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
    if(SDL_RenderCopy(gf2d_graphics_get_renderer(), self->Message, NULL, &self->box)){ //you put the renderer's name first, the Message, the crop size(you can ignore this if you don't want to dabble with cropping), and the rect which is the size and coordinate of your texture
    slog("rendering %s", self->Message);}
    //draw box collider
    gf2d_draw_rect(self->box, vector4d(255,0,255,255));
    // slog("Pos %f.%f", self->position.x, self->position.y);

}

void menu_draw_all()
{
    gf2d_sprite_draw(
        menu_manager.bg,
        vector2d(0,0),
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL);
    int i;
    for (i = 0;i < menu_manager.maxMenus;i++)
    {
        if (!menu_manager.menuList[i]._inuse)continue;
        menu_draw(&menu_manager.menuList[i]);
    }
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
    menu->box = box;
    menu->drawOffset = drawOffset;
    menu->sprite = sprite;
    menu->onClick = think;

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
SDL_Rect box_exit = {
    (LEVEL_WIDTH / 2) - MENU_BUTTON_HALF_WIDTH,
    (LEVEL_HEIGHT / 2) + MENU_BUTTON_HALF_HEIGHT,
    MENU_BUTTON_HALF_WIDTH * 2,
    MENU_BUTTON_HALF_HEIGHT
};