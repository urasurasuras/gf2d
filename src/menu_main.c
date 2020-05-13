#include "menu.h"
#include "menu_main.h"
#include "menu_pause.h"
#include "game.h"
Vector4D menu_noShade = { 255, 255, 255, 255 };

void menu_main_init() {
    get_menu_active()->bg_color = &menu_noShade;
    get_menu_active()->bg = bg_flat;
    scene_get_active()->soundtrack = main_theme;
    gfc_sound_play(scene_get_active()->soundtrack, 0, .2, -1, -1);
    load_sprites_chars();

    //play button
    button_generic(
        box_play,
        vector2d(-100, -250),
        NULL,
        onClick_play,
        Sans,
        "Play"
    );

    // //Exit button
    button_generic(
        box_exit,
        vector2d(-100, -250),
        NULL,
        onClick_exit,
        Sans,
        "Exit"
    );

    //Character select buttons (toggle)
    Button * blue = 
    button_generic(//Blue team player 1
        box_blue_team,
        vector2d(-100, -250),
        sprite_player1,
        onClick_charSwap_blue,
        Sans,
        ""
    );
    blue->colorShift = v4d_blue;
    strcpy(scene_get_active()->bluePlayer, "Player1");

    Button *red = 
    button_generic(//Red team player 1
        box_red_team,
        vector2d(-100, -250),
        sprite_player2,
        onClick_charSwap_red,
        Sans,
        ""
    );
    red->colorShift = v4d_red;
    strcpy(scene_get_active()->redPlayer, "Player2");

    get_menu_active()->type = MENU_MAIN;
    get_menu_active()->_inuse = 1;
    scene_get_active()->menu_manager = get_menu_active();
    scene_get_active()->type = scn_MENU;
}

void onClick_play() {
    Mix_Pause(scene_get_active()->soundtrack->defaultChannel);
    scene_get_active()->soundtrack = grass_theme;
    gfc_sound_play(scene_get_active()->soundtrack, -1, .1, -1, -1);
    //Purge current scene
    //memset(scene_get_active(), 0, sizeof(Scene));
    //Load pause menu
    button_free_all();
    menu_pause_init();
    //scene_get_active()->menu_manager-
    //Load grass level
    scene_get_active()->level = level_new(
        gf2d_sprite_load_image("images/backgrounds/bg_grass.png"),
        bounds_level, bounds_stage_wide, 
        1
    );
    load_sprites_projectiles();
    players_spawn();
    level_pickups_spawn();//FIXME: spawn pickups before main game loop

    scene_get_active()->type = scn_LEVEL;
    get_menu_active()->type = MENU_PAUSE;
    scene_pause_toggle();
    scene_pause_toggle();

    slog("Spawning char %s and %s", scene_get_active()->bluePlayer, scene_get_active()->redPlayer);
}

void onClick_exit(Button* self) {
    if (get_menu_active()->type != MENU_MAIN)return;
    scene_get_active()->done = 1;
}

void onClick_charSwap_blue(Button* self) {

    switch (self->toggle)
    {
    case 1:
        self->sprite = sprite_player1;
        strcpy(scene_get_active()->bluePlayer, "Player1");
        self->toggle = 0;
        break;
    case 0:
        self->sprite = sprite_player3;
        strcpy(scene_get_active()->bluePlayer, "Player3");
        self->toggle = 1;
        break;
    default:
        break;
    }
    slog("Blue player set to %s", scene_get_active()->bluePlayer);
    
}
void onClick_charSwap_red(Button* self) {

    switch (self->toggle)
    {
    case 1:
        self->sprite = sprite_player2;
        strcpy(scene_get_active()->redPlayer, "Player2");
        self->toggle = 0;
        break;
    case 0:
        self->sprite = sprite_player4;
        strcpy(scene_get_active()->redPlayer, "Player4");
        self->toggle = 1;
        break;
    default:
        break;
    }
    slog("Red player set to %s", scene_get_active()->redPlayer);
    
}