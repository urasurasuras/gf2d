#include <stdlib.h>
#include "simple_logger.h"
#include "menu.h"
#include <SDL.h>
#include "gf2d_draw.h"
#include "collision.h"
#include "level.h"

typedef struct 
{
    Uint32  maxMenus;         /**<Maximum number of entities*/
    Menu  *menuList;     /**<List of entities*/
}MenuManager;

static MenuManager menu_manager = {0};

Menu *menu_new(){
    int i;
    for (i=0; i < menu_manager.maxMenus; i++){
        if(menu_manager.menuList[i]._inuse)continue;
        menu_manager.menuList[i]._inuse = 1;
        slog("Items in menu list %d", i);
        return &menu_manager.menuList[i];
    }
    slog("out of open menu slots in memory");
    return NULL;
}

void menu_manager_init(Uint32 maxMenus){
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
    memset(menu_manager.menuList,0,sizeof(Menu)*maxMenus);
    slog("Menu manager initalized");
    atexit(menu_manager_close);
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
    if (self->think){
        self->think(self);
    }
    self->frame = self->frame + 0.1;
    if (self->frame > self->maxFrames)self->frame=0;
    // if (level_bounds_test_circle(level_get_active(), self->position, self->radius))
    // {
    //     //TODO: Do something is ent hits bounds
    // }
    entity_collision_check(self);
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
        (Uint32)self->frame
    );
    //draw box collider
    gf2d_draw_rect(self->box, vector4d(255,0,255,255));
    // slog("Pos %f.%f", self->position.x, self->position.y);

}

void menu_draw_all()
{
    int i;
    for (i = 0;i < menu_manager.maxMenus;i++)
    {
        if (!menu_manager.menuList[i]._inuse)continue;
        menu_draw(&menu_manager.menuList[i]);
    }
}

void button_exit_think (Menu *self){
    int mx,my;
    SDL_GetMouseState(&mx,&my);
    if (collide_menu(self->box, vector2d(mx,my))){
        if (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_LEFT)) {
            level_get_active()->done = 1;
            // SDL_Log("done = %d", done);
        }
    }
}
