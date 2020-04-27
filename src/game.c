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
#include "scene.h"

int main(int argc, char * argv[])
{
    char cwd[64];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        printf("Current working dir: %s\n", cwd);
    }
    else {
        perror("getcwd() error");
        return 1;
    }

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
    menu_manager_init(32, gf2d_sprite_load_image("images/backgrounds/bg_flat.png"));

    	
    if(TTF_Init()==-1) {
        printf("TTF_Init: %s\n", TTF_GetError());
        exit(2);
    }
    Sans = TTF_OpenFont("fonts/bignoodletoo.ttf", 72); //this opens a font style and sets a size
    if (!Sans)slog("no font");
    
    menu_pause_init();
    
    SDL_ShowCursor(SDL_DISABLE);
    //UI cooldowns
    int last_tab = 0;
    
    /*demo setup*/
    bg_grass = gf2d_sprite_load_image("images/backgrounds/bg_grass.png");
    level = level_new(bg_grass, bounds_level, bounds_stage_wide, 1);

    Scene* title = scene_new();
    title->menu_manager = get_menu_active();
    //title->level = level;
    
    projectile_load_sprites();
    players_spawn();
    level_pickups_spawn();//FIXME: spawn pickups before main game loop
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
        
        
        //level_update(level);
        scene_update(title);
    
        gf2d_graphics_clear_screen();// clears drawing buffers
        // all drawing should happen betweem clear_screen and next_frame
            //backgrounds drawn first
            // gf2d_sprite_draw_image(sprite,vector2d(0,0));
            //level_draw(level);
            scene_draw(scene_get_active());

            //UI elements last      
            if (level_get_active()->paused){
                
            }      

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
