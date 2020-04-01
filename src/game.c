#include <SDL.h>
#include <SDL_ttf.h>
#include "gf2d_graphics.h"
#include "gf2d_sprite.h"
#include "gf2d_draw.h"
#include "simple_logger.h"
#include "entity.h"
#include "player.h"
#include "level.h"
#include "menu.h"
#include "projectile.h"

int main(int argc, char * argv[])
{
    /*variable declarations*/

    const Uint8 * keys;
    int mx,my;
    float mf = 0;
    Sprite *mouse;
    // Menu *menu_exit;
    // Menu *menu_save;
    // Menu *menu_level;

    SDL_Rect box_lvl_b = {
        (LEVEL_WIDTH/2)- MENU_BUTTON_HALF_WIDTH, 
        (LEVEL_HEIGHT/2) - MENU_BUTTON_HALF_HEIGHT, 
        MENU_BUTTON_HALF_WIDTH*2, 
        MENU_BUTTON_HALF_HEIGHT
    };
    SDL_Rect box_save = {
        (LEVEL_WIDTH/2)- MENU_BUTTON_HALF_WIDTH, 
        (LEVEL_HEIGHT/2) - MENU_BUTTON_HALF_HEIGHT*2, 
        MENU_BUTTON_HALF_WIDTH*2, 
        MENU_BUTTON_HALF_HEIGHT
    };
    SDL_Rect box_exit = {
        (LEVEL_WIDTH/2)- MENU_BUTTON_HALF_WIDTH, 
        (LEVEL_HEIGHT/2) + MENU_BUTTON_HALF_HEIGHT, 
        MENU_BUTTON_HALF_WIDTH*2, 
        MENU_BUTTON_HALF_HEIGHT
    };
    
    Vector4D mouseColor = {255,100,255,200};

    Level *level;

    /*program initializtion*/
    init_logger("gf2d.log");
    slog("---==== BEGIN ====---");
    gf2d_graphics_initialize(
        "gf2d",
        LEVEL_WIDTH,
        LEVEL_HEIGHT,
        LEVEL_WIDTH,
        LEVEL_HEIGHT,
        vector4d(0,0,0,255),
        0);
    gf2d_graphics_set_frame_delay(16);
    gf2d_sprite_init(1024);
    entity_manager_init(32);
    menu_manager_init(32);

    	
    if(TTF_Init()==-1) {
        printf("TTF_Init: %s\n", TTF_GetError());
        exit(2);
    }
    TTF_Font* Sans = TTF_OpenFont("fonts/bignoodletoo.ttf", 72); //this opens a font style and sets a size
    if (!Sans)slog("no font");
    // //Exit button
    menu_generic(
        box_exit,
        vector2d(-100,-250),
        gf2d_sprite_load_image("images/ui/button.png"),
        button_exit_think,
        Sans,
        "Exit"
    );
    // menu_exit = menu_new();
    // menu_exit->box = box_exit;
    // menu_exit->drawOffset = vector2d(-100,-250);
    // menu_exit->position = vector2d(500,500);
    // menu_exit->sprite = gf2d_sprite_load_image("images/ui/button.png");
    // menu_exit->think = button_exit_think;
    // //save button
    menu_generic(
        box_save,
        vector2d(-100,-250),
        gf2d_sprite_load_image("images/ui/button.png"),
        button_save_think,
        Sans,
        "Save"
    );
    // menu_save = menu_new();
    // menu_save->box = box_save;
    // menu_save->think = button_save_think;
    // //Level button
    menu_generic(
        box_lvl_b,
        vector2d(-100,-250),
        gf2d_sprite_load_image("images/ui/button.png"),
        button_level_think,
        Sans,
        "Next level"
    );
    // menu_level = menu_new();
    // menu_level->box = box_lvl_b;
    // menu_level->think = button_level_think;


    SDL_ShowCursor(SDL_DISABLE);
    //UI cooldowns
    int last_tab = 0;
    
    /*demo setup*/
    bg_grass = gf2d_sprite_load_image("images/backgrounds/bg_grass.png");
    level = level_new(bg_grass, bounds_level,bounds_stage_wide,1);
    mouse = gf2d_sprite_load_all("images/pointer.png",32,32,16);
    projectile_load_sprites();
    players_spawn();
    if(TTF_Init()==-1) {
        printf("TTF_Init: %s\n", TTF_GetError());
        exit(2);
    }

    
    /*main game loop*/
    while(!level_get_active()->done)
    {
        SDL_PumpEvents();   // update SDL's internal event structures
        keys = SDL_GetKeyboardState(NULL); // get the keyboard state for this frame
        
        //
        /*update things here*/
        SDL_GetMouseState(&mx,&my);
        mf+=0.1;
        if (mf >= 16.0)mf = 0;

        if (!level_get_active()->paused)
        entity_update_all();
    
        gf2d_graphics_clear_screen();// clears drawing buffers
        // all drawing should happen betweem clear_screen and next_frame
            //backgrounds drawn first
            // gf2d_sprite_draw_image(sprite,vector2d(0,0));
            level_draw(level);
            //Draw entities
            entity_draw_all();
     level_pickups_spawn();

            //UI elements last      
            if (level_get_active()->paused){
                menu_update_all();
                menu_draw_all();
                // slog("Paused");
            gf2d_sprite_draw(
                mouse,
                vector2d(mx,my),
                NULL,
                NULL,
                NULL,
                NULL,
                &mouseColor,
                (int)mf);}      

        gf2d_grahics_next_frame();// render current draw frame and skip to the next frame
        
        if (keys[SDL_SCANCODE_TAB] && last_tab + 750 < SDL_GetTicks()){
            last_tab = SDL_GetTicks();
            slog("tab");
            if (level_get_active()->paused)level_get_active()->paused = 0;
            else if (!level_get_active()->paused) level_get_active()->paused = 1;
        }
        // slog("Tick: %d", SDL_GetTicks());
        if (keys[SDL_SCANCODE_ESCAPE])level_get_active()->done = 1; // exit condition
        // slog("Rendering at %f FPS",gf2d_graphics_get_frames_per_second());
    }
    slog("---==== END ====---");
    level_free(level);
    return 0;
}
/*eol@eof*/
