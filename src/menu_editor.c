#include "menu_editor.h"
#include "menu_main.h"

void menu_editor_init() {
    // //Exit button
    menu_generic(
        ed_box_exit,
        vector2d(-100, -250),
        gf2d_sprite_load_image("images/ui/button.png"),
        onClick_exit,
        Sans,
        "Exit"
    );
    //get_menu_active()->type = MENU_MAIN;
    get_menu_active()->_inuse = 1;
    scene_get_active()->menu_manager = get_menu_active();
}

SDL_Rect ed_box_exit = { 
    0, 
    LEVEL_HEIGHT,
    EDITOR_MENU_HEIGHT*3,
    EDITOR_MENU_HEIGHT
};