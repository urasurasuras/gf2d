#include "menu.h"
#include "menu_main.h"
#include "menu_pause.h"

void menu_main_init() {
    //
    menu_manager_init(8, gf2d_sprite_load_image("images/backgrounds/bg_flat.png"));

    //play button
    menu_generic(
        box_save,
        vector2d(-100, -250),
        gf2d_sprite_load_image("images/ui/button.png"),
        onClick_play,
        Sans,
        "Play"
    );

    ////Level button
    //menu_generic(
    //    box_lvl_b,
    //    vector2d(-100, -250),
    //    gf2d_sprite_load_image("images/ui/button.png"),
    //    button_level_think,
    //    Sans,
    //    "Next level"
    //);

    // //Exit button
    menu_generic(
        box_exit,
        vector2d(-100, -250),
        gf2d_sprite_load_image("images/ui/button.png"),
        onClick_exit,
        Sans,
        "Exit"
    );
    get_menu_active()->_inuse = 1;
    scene_get_active()->menu_manager = get_menu_active();
}

void onClick_play() {
    //Purge current scene

    //Load grass level
    scene_get_active()->level = level_new(
        gf2d_sprite_load_image("images/backgrounds/bg_grass.png"),
        bounds_level, bounds_stage_wide, 
        1
    );
    projectile_load_sprites();
    players_spawn();
    level_pickups_spawn();//FIXME: spawn pickups before main game loop

    slog("Clicked play on %d", SDL_GetTicks());
}
