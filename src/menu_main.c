#include "menu.h"
#include "menu_main.h"
#include "menu_pause.h"
#include "game.h"
Vector4D menu_noShade = { 255, 255, 255, 255 };

void menu_main_init() {
    get_menu_active()->bg_color = &menu_noShade;

    gfc_sound_play(main_theme, 0, .2, -1, -1);

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
    scene_get_active()->type = scn_MENU;
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
    get_menu_active()->type = MENU_PAUSE;
    scene_pause_toggle();
    scene_pause_toggle();

    slog("Clicked play on %d", SDL_GetTicks());
}

void onClick_exit(Menu* self) {
    if (get_menu_active()->type != MENU_MAIN)return;
    scene_get_active()->done = 1;
}