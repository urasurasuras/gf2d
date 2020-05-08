#include <game.h>
#include "gf2d_graphics.h"
#include "gf2d_sprite.h"
#include "gf2d_draw.h"
#include "simple_logger.h"
#include "entity.h"
#include "player.h"
#include "level.h"
#include "menu.h"
#include "menu_main.h"
#include "projectile.h"
#include "scene.h"

int main(int argc, char * argv[])
{
    /*program initializtion*/

    //Init font
    if (TTF_Init() == -1) {
        printf("TTF_Init: %s\n", TTF_GetError());
        exit(2);
    }
    Sans = TTF_OpenFont("fonts/bignoodletoo.ttf", 72); //this opens a font style and sets a size
    if (!Sans)slog("no font");

    SDL_ShowCursor(SDL_DISABLE);
    //UI cooldowns
    int last_tab = 0;

    init_logger("gf2d.log");
    slog("---==== BEGIN ====---");
    if (argc == 2) {
        editorMode = 1;
        gf2d_graphics_initialize(
            "gf2d-editor",
            LEVEL_WIDTH,
            LEVEL_HEIGHT+ EDITOR_MENU_HEIGHT,
            LEVEL_WIDTH,
            LEVEL_HEIGHT+ EDITOR_MENU_HEIGHT,
            vector4d(0, 0, 0, 255),
            0);


        gf2d_graphics_set_frame_delay(16);
        gf2d_sprite_init(1024);
        menu_manager_init(8, NULL);
        get_menu_active()->mouse_sprite = gf2d_sprite_load_all("images/pointer.png", 32, 32, 16);
        get_menu_active()->mouse_color = vector4d(255, 255, 255, 255);
        get_menu_active()->mf = 0;

        menu_editor_init();
        scene_get_active()->level = level_blank(gf2d_sprite_load_image("images/backgrounds/bg_grass.png"));

    }
    else
    {
        editorMode = 0;

        gf2d_graphics_initialize(
            "gf2d",
            LEVEL_WIDTH,
            LEVEL_HEIGHT,
            LEVEL_WIDTH,
            LEVEL_HEIGHT,
            vector4d(0, 0, 0, 255),
            0);


        gf2d_graphics_set_frame_delay(16);
        gf2d_sprite_init(1024);
        menu_manager_init(8, gf2d_sprite_load_image("images/backgrounds/bg_flat.png"));
        get_menu_active()->mouse_sprite = gf2d_sprite_load_all("images/pointer.png", 32, 32, 16);
        get_menu_active()->mouse_color = vector4d(255, 255, 255, 255);
        get_menu_active()->mf = 0;

        gfc_audio_init(
            8,
            8,
            8,
            8,
            1,
            1
        );


        scene_new();
        Scene* title = scene_get_active();
        menu_main_init();
        //menu_pause_init();
        //title->menu_manager = get_menu_active();
        //title->level = level;


    }
    
    
    
    /*demo setup*/



    
    /*main game loop*/
    while(!scene_get_active()->done)
    {
        SDL_PumpEvents();   // update SDL's internal event structures
        keys = SDL_GetKeyboardState(NULL); // get the keyboard state for this frame
        
        /*update things here*/        
        scene_update();
    
        gf2d_graphics_clear_screen();// clears drawing buffers
        // all drawing should happen betweem clear_screen and next_frame
            //backgrounds drawn first
            // gf2d_sprite_draw_image(sprite,vector2d(0,0));
            //level_draw(level);
            scene_draw();

            //UI elements last      
            if (level_get_active()->paused){
                
            }      

        gf2d_grahics_next_frame();// render current draw frame and skip to the next frame
        
        if (keys[SDL_SCANCODE_TAB] && last_tab + 750 < SDL_GetTicks()){
            last_tab = SDL_GetTicks();
            slog("tab");
            
            SDL_SetWindowFullscreen(gf2d_get_mainWin(), SDL_WINDOW_FULLSCREEN);
            //SDL_SetWindowDisplayMode(m_sdlWindow, &m_fullscreenDisplayMode);

            //SDL_GetWindowData(SDL_GL_GetCurrentWindow(), "gf2d");
            


            //if (level_get_active()->paused)level_get_active()->paused = 0;
            //else if (!level_get_active()->paused) level_get_active()->paused = 1;
        }
        // slog("Tick: %d", SDL_GetTicks());
        //if (keys[SDL_SCANCODE_ESCAPE])scene_get_active()->menu_manager->_inuse = 0; // exit condition
        // slog("Rendering at %f FPS",gf2d_graphics_get_frames_per_second());
    }
    slog("---==== END ====---");
    //free(scene_get_active());
    return 0;
}
/*eol@eof*/
