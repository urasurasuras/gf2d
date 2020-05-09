#include "scene_end.h"

void end_game(int winner) {
    menu_free_all();
    scene_pause_toggle();

    displayWinner(winner);

    //Exit button
    menu_generic(
        box_backToMain,
        vector2d(-100, -250),
        gf2d_sprite_load_image("images/ui/button.png"),
        onClick_backToMain,
        Sans,
        "Main Menu"
    );
}

void displayWinner(int winner)
{
    char winnerText[32];
    if (winner == TEAM_A) {
        snprintf(winnerText, 32, "Blue team wins !");
    }
    else
    {
        snprintf(winnerText, 32, "Red team wins !");
    }

    menu_generic(
        box_play,
        vector2d(-100, -250),
        NULL,
        NULL,
        Sans,
        winnerText
    );
}

