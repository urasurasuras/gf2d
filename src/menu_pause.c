#include "menu_pause.h"

void menu_pause_init() {

    //save button
    menu_generic(
        box_save,
        vector2d(-100, -250),
        gf2d_sprite_load_image("images/ui/button.png"),
        onClick_save,
        Sans,
        "Save"
    );

    // //Level button
    menu_generic(
        box_lvl_b,
        vector2d(-100, -250),
        gf2d_sprite_load_image("images/ui/button.png"),
        onClick_level,
        Sans,
        "Next level"
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
}


void onClick_exit(Menu* self) {
    scene_get_active()->done = 1;
}

void onClick_save(Menu* self) {
    //static int last_save = 0;
    //// slog("%d: Last saved: %d", SDL_GetTicks(), last_save);
    //int mx, my;
    //SDL_GetMouseState(&mx, &my);
    //if (collide_menu(self->box, vector2d(mx, my))) {
    //    if (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_LEFT) && last_save + 750 < SDL_GetTicks()) {
    //        last_save = SDL_GetTicks();
    SJson* allData;
    SJson* playerData;
    SJson* player_object;
    SJson* player_name;
    SJson* player_pos;
    SJson* player_health;

    allData = sj_object_new();
    playerData = sj_array_new();
    int i;
    for (i = 0; i < entity_manager_get_active()->maxEnts; i++)
    {
        if (entity_manager_get_active()->entityList[i].type == ENT_PLAYER) {

            Entity current_ent = entity_manager_get_active()->entityList[i];
            // Player *current_player = (Player *)current_ent.typeOfEnt;

            player_object = sj_object_new();
            player_pos = sj_array_new();
            player_health = sj_new_float(current_ent.health);

            sj_array_append(player_pos, sj_new_float(current_ent.position.x));
            sj_array_append(player_pos, sj_new_float(current_ent.position.y));

            player_name = sj_new_str(current_ent.name);

            sj_object_insert(player_object, "Name", player_name);
            sj_object_insert(player_object, "Position", player_pos);
            sj_object_insert(player_object, "Health", player_health);

            sj_array_append(playerData, player_object);

            // slog("Name: %s", entity_manager_get_active()->entityList[i].name);
        }

    }

    sj_object_insert(allData, "Players", playerData);
    // SJson *test_json;
    // SJson *test_json_value;
    // SJString *json_test_string;
    // SJson *sub;


    // json_test_string = sj_string_new();
    // sj_string_append(json_test_string, "test_value");

    // test_json_value = sj_string_to_value(json_test_string);
    // sj_echo(test_json_value);

    // sj_object_insert(test_json, "test_key", test_json_value);

    // slog("json created:");
    sj_echo(allData);
    sj_save(allData, "data/player.save");
    sj_free(allData);
    // slog("saved");
        //}
    //}
}

void onClick_level(Menu* self) {
    //static int last_level_change = 0;
    //int mx, my;
    //SDL_GetMouseState(&mx, &my);
    //if (collide_menu(self->box, vector2d(mx, my))) {
    //    if (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_LEFT) && last_level_change + 750 < SDL_GetTicks()) {
    //        last_level_change = SDL_GetTicks();
    if (level_get_active()->level_type == LEVEL_T_NORMAL) {
        gf2d_sprite_free(level_get_active()->background);
        level_get_active()->background = gf2d_sprite_load_image("images/backgrounds/bg_lava.png");
        level_get_active()->bounds_stage = bounds_stage_narrow;
        level_get_active()->level_type = LEVEL_T_LAVA;
        level_get_active()->neutral_monster->_inuse = 0;
        level_get_active()->neutral_monster = lavaGuy_new();

        slog("Level type: %d", level_get_active()->level_type);
    }
    else if (level_get_active()->level_type == LEVEL_T_LAVA) {
        gf2d_sprite_free(level_get_active()->background);
        level_get_active()->background = gf2d_sprite_load_image("images/backgrounds/bg_grass.png");
        level_get_active()->bounds_stage = bounds_stage_wide;
        level_get_active()->level_type = LEVEL_T_NORMAL;
        level_get_active()->neutral_monster->_inuse = 0;
        level_get_active()->neutral_monster = grassGuy_new();

        slog("Level type: %d", level_get_active()->level_type);
    }
    // SDL_Log("done = %d", done);
    //    }
    //}
}