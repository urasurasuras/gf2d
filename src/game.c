#include <SDL.h>
#include "gf2d_graphics.h"
#include "gf2d_sprite.h"
#include "gf2d_draw.h"
#include "simple_logger.h"
#include "entity.h"
#include "player.h"
#include "level.h"
#include "menu.h"
#include "mouse.h"

int main(int argc, char * argv[])
{
    /*variable declarations*/
    int done = 0;
    int paused = 0;
    const Uint8 * keys;
    int mx,my;
    float mf = 0;
    Mouse *mouse;
    
    Vector4D mouseColor = {255,100,255,200};
    SDL_Rect bounds = {0,0,LEVEL_WIDTH,LEVEL_HEIGHT};

    Level *level;

    /*program initializtion*/
    init_logger("gf2d.log");
    slog("---==== BEGIN ====---");
    gf2d_graphics_initialize(
        "gf2d",
        1280,
        720,
        1280,
        720,
        vector4d(0,0,0,255),
        0);
    gf2d_graphics_set_frame_delay(16);
    gf2d_sprite_init(1024);
    entity_manager_init(32);
    menu_manager_init(32);
    mouse = mouse_new();
    SDL_ShowCursor(SDL_DISABLE);
    
    /*demo setup*/
    level = level_new("images/backgrounds/90b.png",bounds);
    mouse->sprite = gf2d_sprite_load_all("images/pointer.png",32,32,16);
    players_spawn();
    /*main game loop*/
    while(!done)
    {
        SDL_PumpEvents();   // update SDL's internal event structures
        keys = SDL_GetKeyboardState(NULL); // get the keyboard state for this frame
        
        //
        /*update things here*/
        SDL_GetMouseState(&mx,&my);

        mouse->position = vector2d(mx,my);
        mouse->frame+=0.1;
        if (mouse->frame >= 16.0)mouse->frame = 0;
        entity_update_all();
           
        gf2d_graphics_clear_screen();// clears drawing buffers
        // all drawing should happen betweem clear_screen and next_frame
            //backgrounds drawn first
            // gf2d_sprite_draw_image(sprite,vector2d(0,0));
            level_draw(level);
            //Draw entities
            entity_draw_all();

            //UI elements last
            if (!paused){
            gf2d_sprite_draw(
                mouse->sprite,
                vector2d(mx,my),
                NULL,
                NULL,
                NULL,
                NULL,
                &mouseColor,
                (int)mf);}
        gf2d_grahics_next_frame();// render current draw frame and skip to the next frame
        
        if (keys[SDL_SCANCODE_TAB]){
            slog("tab");
            if (paused)paused = 0;
            else if (!paused) paused = 1;
        }
        if (keys[SDL_SCANCODE_ESCAPE])done = 1; // exit condition
        // slog("Rendering at %f FPS",gf2d_graphics_get_frames_per_second());
    }
    slog("---==== END ====---");
    level_free(level);
    mouse_free(mouse);
    return 0;
}
/*eol@eof*/
