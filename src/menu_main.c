#include "menu.h"
#include "menu_main.h"
#include "menu_pause.h"
#include "game.h"

void menu_main_init() {
    
    gfc_sound_play(gfc_sound_load("audio/njit-theme-song.mp3", .1, 0), 0, .2, -1, -1);

    //play button
    menu_generic(
        box_play,
        vector2d(-100, -250),
        gf2d_sprite_load_image("images/ui/button.png"),
        onClick_play,
        Sans,
        "Play"
    );

    // //Exit button
    menu_generic(
        box_exit,
        vector2d(-100, -250),
        gf2d_sprite_load_image("images/ui/button.png"),
        onClick_exit,
        Sans,
        "Exit"
    );
    get_menu_active()->type = MENU_MAIN;
    get_menu_active()->_inuse = 1;
    scene_get_active()->menu_manager = get_menu_active();
}

void onClick_play() {
    //Purge current scene
    //memset(scene_get_active(), 0, sizeof(Scene));
    //Load pause menu
    menu_free_all();
    menu_pause_init();
    //scene_get_active()->menu_manager-
    //Load grass level
    scene_get_active()->level = level_new(
        gf2d_sprite_load_image("images/backgrounds/bg_grass.png"),
        bounds_level, bounds_stage_wide, 
        1
    );
    projectile_load_sprites();
    players_spawn();
    level_pickups_spawn();//FIXME: spawn pickups before main game loop

    scene_get_active()->type = scn_LEVEL;

    slog("Clicked play on %d", SDL_GetTicks());
}

void onClick_exit(Menu* self) {
    scene_get_active()->done = 1;
}