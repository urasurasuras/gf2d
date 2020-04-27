#include "menu_pause.h"


void menu_pause_init() {

    SDL_Rect box_lvl_b = {
        (LEVEL_WIDTH / 2) - MENU_BUTTON_HALF_WIDTH,
        (LEVEL_HEIGHT / 2) - MENU_BUTTON_HALF_HEIGHT,
        MENU_BUTTON_HALF_WIDTH * 2,
        MENU_BUTTON_HALF_HEIGHT
    };
    SDL_Rect box_save = {
        (LEVEL_WIDTH / 2) - MENU_BUTTON_HALF_WIDTH,
        (LEVEL_HEIGHT / 2) - MENU_BUTTON_HALF_HEIGHT * 2,
        MENU_BUTTON_HALF_WIDTH * 2,
        MENU_BUTTON_HALF_HEIGHT
    };
    SDL_Rect box_exit = {
        (LEVEL_WIDTH / 2) - MENU_BUTTON_HALF_WIDTH,
        (LEVEL_HEIGHT / 2) + MENU_BUTTON_HALF_HEIGHT,
        MENU_BUTTON_HALF_WIDTH * 2,
        MENU_BUTTON_HALF_HEIGHT
    };

    //save button
    menu_generic(
        box_save,
        vector2d(-100, -250),
        gf2d_sprite_load_image("images/ui/button.png"),
        button_save_think,
        Sans,
        "Save"
    );

    // //Level button
    menu_generic(
        box_lvl_b,
        vector2d(-100, -250),
        gf2d_sprite_load_image("images/ui/button.png"),
        button_level_think,
        Sans,
        "Next level"
    );

    // //Exit button
    menu_generic(
        box_exit,
        vector2d(-100, -250),
        gf2d_sprite_load_image("images/ui/button.png"),
        button_exit_think,
        Sans,
        "Exit"
    );


}