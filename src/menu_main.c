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
        NULL,
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
        button_exit_think,
        Sans,
        "Exit"
    );

    scene_get_active()->menu_manager = get_menu_active();
}
