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
    menu->think = think;

    SDL_Color White = {255, 255, 255};  // this is the color in rgb format, maxing out all would give you the color white, and it will be your text's color

    SDL_Surface* surfaceMessage = TTF_RenderText_Solid(Sans, text, White); // as TTF_RenderText_Solid could only be used on SDL_Surface then you have to create the surface first

    SDL_Texture* Message = SDL_CreateTextureFromSurface(gf2d_graphics_get_renderer(), surfaceMessage); //now you can convert it into a texture

    menu->Message = Message;
    return menu;
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

void button_save_think (Menu *self){
    static int last_save = 0;
    // slog("%d: Last saved: %d", SDL_GetTicks(), last_save);
    int mx,my;
    SDL_GetMouseState(&mx,&my);
    if (collide_menu(self->box, vector2d(mx,my))){
        if (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_LEFT) && last_save + 750 < SDL_GetTicks()) {
            last_save = SDL_GetTicks();
            SJson *allData;
            SJson *playerData;
            SJson *player_object;
            SJson *player_name;
            SJson *player_pos;
            SJson *player_health;

            allData = sj_object_new();
            playerData = sj_array_new();
            int i;
            for (i = 0;i < entity_manager_get_active().maxEnts;i++)
            {
                if (entity_manager_get_active().entityList[i].type == ENT_PLAYER){
                    
                    Entity current_ent = entity_manager_get_active().entityList[i];
                    // Player *current_player = (Player *)current_ent.typeOfEnt;

                    player_object = sj_object_new();
                    player_pos = sj_array_new();
                    player_health = sj_new_float(current_ent.health);
                    
                    sj_array_append(player_pos,sj_new_float(current_ent.position.x));
                    sj_array_append(player_pos,sj_new_float(current_ent.position.y));

                    player_name = sj_new_str(current_ent.name);

                    sj_object_insert(player_object, "Name", player_name);
                    sj_object_insert(player_object, "Position", player_pos);
                    sj_object_insert(player_object, "Health", player_health);

                    sj_array_append(playerData, player_object);

                    // slog("Name: %s", entity_manager_get_active().entityList[i].name);
                }
                
            }

            sj_object_insert(allData, "Players", playerData);
            // SJson *test_json;
            // SJson *test_json_value;
            // SJString *json_test_string;
            // SJson *sub;
            
          
            // json_test_string = sj_string_new();
            // sj_string_append(json_test_string, "test_value");

            // test_json_value = sj_string_to_value(json_test_string);
            // sj_echo(test_json_value);

            // sj_object_insert(test_json, "test_key", test_json_value);

            // slog("json created:");
            sj_echo(allData);
            sj_save(allData, "data/player.save");
            sj_free(allData);
            // slog("saved");
        }
    }
}

void button_level_think (Menu *self){
    static int last_level_change = 0;
    int mx,my;
    SDL_GetMouseState(&mx,&my);
    if (collide_menu(self->box, vector2d(mx,my))){
        if (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_LEFT) && last_level_change + 750 < SDL_GetTicks()) {
            last_level_change = SDL_GetTicks();
            if (level_get_active()->level_type == LEVEL_T_NORMAL){
                gf2d_sprite_free(level_get_active()->background);
                level_get_active()->background = gf2d_sprite_load_image("images/backgrounds/bg_lava.png");
                level_get_active()->bounds_stage = bounds_stage_narrow;
                level_get_active()->level_type = LEVEL_T_LAVA;
                level_get_active()->neutral_monster->_inuse = 0;
                level_get_active()->neutral_monster = lavaGuy_new();

                slog("Level type: %d", level_get_active()->level_type);
            }
            else if (level_get_active()->level_type == LEVEL_T_LAVA){
                gf2d_sprite_free(level_get_active()->background);
                level_get_active()->background = gf2d_sprite_load_image("images/backgrounds/bg_grass.png");
                level_get_active()->bounds_stage = bounds_stage_wide;
                level_get_active()->level_type = LEVEL_T_NORMAL;
                level_get_active()->neutral_monster->_inuse = 0;
                level_get_active()->neutral_monster = grassGuy_new();

                slog("Level type: %d", level_get_active()->level_type);
            }
            // SDL_Log("done = %d", done);
        }
    }
}
