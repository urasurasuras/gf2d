#include "player.h"

Vector2D spawn_top_left     = {LEVEL_SPAWN_OFFSET, LEVEL_SPAWN_OFFSET};
Vector2D spawn_top_right    = {LEVEL_WIDTH - LEVEL_SPAWN_OFFSET, LEVEL_SPAWN_OFFSET};
Vector2D spawn_bottom_left  = {LEVEL_SPAWN_OFFSET, LEVEL_HEIGHT - LEVEL_SPAWN_OFFSET};
Vector2D spawn_bottom_right = {LEVEL_WIDTH - LEVEL_SPAWN_OFFSET, LEVEL_HEIGHT - LEVEL_SPAWN_OFFSET};

Sprite *p0;
Sprite *p1;
Sprite *p2;
Sprite *p3;

SJson *saveFile;
SJson *pArray_save;
SJson *pArray_config;

void players_spawn(){

    // Sprite player_sprite_array[4];
    SJson *cfgFile = level_get_active()->config;

    saveFile = sj_load("data/player.save");

    if (!cfgFile){
        slog("no config file, exiting...");
        return;
    }else
    {
        pArray_config = sj_object_get_value(cfgFile, "Players");
        if (saveFile){
            pArray_save = sj_object_get_value(saveFile, "Players");
            slog("Loading save file...");
        }

        int i;
        SJson *saved_player_data;//Data holder per player
        SJson *config_player_data;//Config holder per player

        SJson *player_name;
        SJson *player_spritePath;
        SJson *player_controller;
        SJson *player_pos;
        // SJson *player_health;
        SJson *player_cooldowns;
        SJson *player_cldn1;
        SJson *player_cldn2;
        SJson *player_cldn3;
        SJson *player_team;

        TextWord *name;
        char* spritePath_string;
        int contIndex;
        Vector2D pos;
        // float *hp;
        int cldn1;
        int cldn2;
        int cldn3;
        int team;
        
        for (i = 0;i < sj_array_get_count(pArray_config);i++){
            config_player_data = sj_array_get_nth(pArray_config, i);

            player_spritePath = sj_object_get_value(config_player_data, "Sprite");
            player_controller = sj_object_get_value(config_player_data, "ControllerIndex");
            player_cooldowns  = sj_object_get_value(config_player_data, "Cooldowns");
            player_team       = sj_object_get_value(config_player_data, "Team");

            //Get cooldowns
       

            player_cldn1 = sj_array_get_nth(player_cooldowns, 0);
            player_cldn2 = sj_array_get_nth(player_cooldowns, 1);
            player_cldn3 = sj_array_get_nth(player_cooldowns, 2);

            if (pArray_save){
                saved_player_data = sj_array_get_nth(pArray_save, i);

                if (saved_player_data){
                    // slog("Existing player");
                    player_name = sj_object_get_value(saved_player_data, "Name");
                    player_pos = sj_object_get_value(saved_player_data, "Position");
                    // player_health = sj_object_get_value(saved_player_data, "Health");
                }
                else
                {
                    // slog("New player");

                    player_name = sj_object_get_value(config_player_data, "Name");
                    player_pos = sj_object_get_value(config_player_data, "Position");
                    // player_health = sj_object_get_value(config_player_data, "Health");
                }
                
         
            }
            else
            {
                slog("New player");

                player_name = sj_object_get_value(config_player_data, "Name");
                player_pos = sj_object_get_value(config_player_data, "Position");
                // player_health = sj_object_get_value(config_player_data, "Health");
            }

            //Set name
            name = (TextWord *)sj_get_string_value(player_name);

            //Set pos
            SJson *posX;
            SJson *posY;
            posX = sj_array_get_nth(player_pos, 0);
            posY = sj_array_get_nth(player_pos, 1);
            sj_get_float_value(posX, &pos.x);
            sj_get_float_value(posY, &pos.y);

            //Set controller index
            sj_get_integer_value(player_controller, &contIndex);
            //Set sprite path
            spritePath_string = (char *)sj_get_string_value(player_spritePath);

            //Set cooldowns
            sj_get_integer_value(player_cldn1, &cldn1);
            sj_get_integer_value(player_cldn2, &cldn2);
            sj_get_integer_value(player_cldn3, &cldn3);

            //Set team
            sj_get_integer_value(player_team, &team);

            //Create player
            player_generic(
                (char *)name,
                i+1, 
                SHAPE_CIRCLE, 
                50, 
                vector2d(-50,-50), 
                1, 
                gf2d_sprite_load_image(spritePath_string), 
                pos, 
                SDL_GameControllerOpen(contIndex), 
                player_think_1, 
                player_touch, 
                cldn1,
                cldn2,
                cldn3,
                team
            );

            // slog("%s",name);
            // sj_echo(player_pos);
            // sj_echo(player_health);       
        }

        // sj_free(saveFile);
        // sj_free(cfgFile);
        // sj_free(pArray_save);
        // sj_free(pArray_config);
        // sj_free(saved_player_data);
        // sj_free(config_player_data);
    }   
}

Entity *player_generic(
    TextWord name,
    int char_index, 
    int collider_shape,
    int radius,
    Vector2D draw_offset,
    float default_speed, 
    Sprite *sprite,
    Vector2D init_pos,
    SDL_GameController *controller,
    void (*think)(struct Entity_S *self),
    void (*touch)(struct Entity_S *self, struct Entity_S *other),
    int cldn_skill1,
    int cldn_skill2,
    int cldn_skill3,
    int team
    )
    {
    Entity *self;
    self = entity_new();
    if (!self)return NULL;
    //Init ent
    strcpy(self->name, name);
    self->sprite = sprite;
    //
    self->collider_shape = collider_shape;
    self->radius_body = radius;
    self->size.x = 32;
    self->size.y = 32;
    vector2d_set(self->drawOffset,-50,-50);
    vector2d_copy(self->position, init_pos);   
    //
    self->think = think;
    self->touch = touch;
    self->maxFrames = 1;
    self->ui_box.w = 50;
    self->ui_box.h = 50;
    self->ui_box.x = (int)self->position.x + draw_offset.x;
    self->ui_box.y = (int)self->position.y + draw_offset.y;
    switch (team)
    {
    case TEAM_A:
        self->color = &v4d_blue;
        break;
    case TEAM_B:
        self->color = &v4d_red;
        break;
    default:
        slog("No team for %s", self->name);
        break;
    }

    //Declaration of player
    Player *player;
    player = (Player *)malloc(sizeof(Player));
    //Init player
    player->controller = controller;
    player->freeze = 0;
    player->cldn_skill1 = cldn_skill1;
    player->cldn_skill2 = cldn_skill2;
    player->cldn_skill3 = cldn_skill3;
    self->health = 100;
    player->index = char_index;
    player->last_skill1 = 0;
    player->last_skill2 = 0;
    player->last_skill3 = 0;
    player->last_skill4 = 0;
    player->last_action1 = 0;
    player->cldn_action1 = 50;
    player->companion = (Entity *)malloc(sizeof(Entity));
    player->deployables = 0;
    // slog("Player created with index: %d", player->index);
    player->speed = default_speed;
    player->strength = 1;
    self->typeOfEnt = (Player *)player;
    self->type = ENT_PLAYER;
    self->team = team;

    if (char_index == 1){
        player->companion = 
        companion_generic(
            self,
            "MusicBee",
            companion_musicBee,
            SHAPE_CIRCLE,
            14,
            100,
            vector2d(-14,-14),
            vector2d(50,-50),
            1,
            0,
            think_behavior,
            musicBee_touch,
            musicBee_detect
        );
    }
    if (char_index == 4){
        player->companion = 
        companion_generic(
            self,
            "LucioAura",
            lucio_aura,
            SHAPE_CIRCLE,
            256,
            0,
            vector2d(-256,-256),
            vector2d(0, 0),
            .01,
            50,
            think_behavior,
            lucioAura_touch,
            NULL
        );
    }
    return self;
}

void player_think_1 (Entity *self){
    static Uint32 last_freeze;
    Player *p = (Player *)self->typeOfEnt;
    Companion *companion = (Companion *)p->companion->typeOfEnt;
    // SDL_GameController *c = p->controller;
    Vector2D vScale;
    Vector2D vScaled;    
    self->ui_box.x = (int)self->position.x + self->drawOffset.x/2;
    self->ui_box.y = (int)self->position.y + self->drawOffset.y/2;
    // self->ui_box.x = (int)self->position.x;
    // self->ui_box.y = (int)self->position.y;
    // if (p->health){
        
    // }


    float x = SDL_GameControllerGetAxis(p->controller, SDL_CONTROLLER_AXIS_LEFTX)/ANALOG_SCALE;
    float y = SDL_GameControllerGetAxis(p->controller, SDL_CONTROLLER_AXIS_LEFTY)/ANALOG_SCALE;
    
    if (level_bounds_test_circle(level_get_active()->bounds_level, self->position, self->radius_body))
    {
        //TODO: Do something is ent hits bounds
        self->position.x -= x*p->speed;
        self->position.y -= y*p->speed;
    }
    if (level_bounds_test_circle(level_get_active()->bounds_stage, self->position, self->radius_body)){
        if (level_get_active()->level_type == LEVEL_T_LAVA){
            self->health -= 0.1;
            // slog("%s health: %f", self->name, p->health);
        }
        else if (level_get_active()->level_type == LEVEL_T_NORMAL){
            p->speed = 0.5;
        }
    }

    if (p->last_action1 + p->cldn_action1 < level_get_active()->frame){
        switch (p->index)
        {
        case 1:
            /* code */
            break;
        case 2:
            /* code */
            break;
        case 3:
            p->speed = 1;
            break;
        case 4:
            slog("set to nonrma");
            companion->strength = .01;
            break;
        default:
            break;
        }
        p->last_action1 = level_get_active()->frame;
    }

    if (self->health <= 0){
        if (p->companion){
            p->companion->_inuse = 0;
        }
        self->_inuse = 0;
        //TODO: not free
    }else if (self->health >= 200)
    {
        self->health = 200;
    }
    
    // slog("%d", last_freeze);
    if (last_freeze + 250 < SDL_GetTicks()){
        switch (p->index)
        {
        case 1:
            if (keys[SDL_SCANCODE_F1]){
                if (p->freeze)p->freeze = 0;
                else p->freeze = 1;
                last_freeze = SDL_GetTicks();
            }
            break;
        case 2:
            if (keys[SDL_SCANCODE_F2]){
                if (p->freeze)p->freeze = 0;
                else p->freeze = 1;
                last_freeze = SDL_GetTicks();
            }
            break;
        case 3:
            if (keys[SDL_SCANCODE_F3]){
                if (p->freeze)p->freeze = 0;
                else p->freeze = 1;
                last_freeze = SDL_GetTicks();
            }
            break;
        case 4:
            if (keys[SDL_SCANCODE_F4]){
                if (p->freeze)p->freeze = 0;
                else p->freeze = 1;
                last_freeze = SDL_GetTicks();
            }
            break;
        default:
            break;
        }
    }
    // if (keys[SDL_SCANCODE_TAB] && last_tab + 750 < SDL_GetTicks()){
    //         last_tab = SDL_GetTicks();
    //         slog("tab");
    //         if (level_get_active()->paused)level_get_active()->paused = 0;
    //         else if (!level_get_active()->paused) level_get_active()->paused = 1;
    //     }

    if (p->freeze)return;

    //Direction update
    if (abs(x) < DEADZONE && abs(y) < DEADZONE){
        //If stick is at origin
        //dont update direction
    }else{
        // slog("%f",vector2d_angle(vector2d(x,y)));
        self->size.x = cos(vector2d_angle(vector2d(x,y)) * M_PI/180);
        self->size.y = sin(vector2d_angle(vector2d(x,y)) * M_PI/180);
        // self->size.x = x;
        // self->size.y = y;
    }
    //Movement
    // slog("Speed of %s: %f", self->name, p->speed);
    self->position.x += x*p->speed;
    self->position.y += y*p->speed;

    if (SDL_GameControllerGetButton(p->controller, SDL_CONTROLLER_BUTTON_A) && p->last_skill1 + p->cldn_skill1 < level_get_active()->frame){
        // slog("Char: %d",p->index);
        switch (p->index)
        {
            case 1:
            hitscan_generic(
                self,
                "Hitscan",
                SHAPE_LINE,
                .1,
                2,
                0,
                rayscan_think,
                hitscan_touch,
                ENT_RAYSCAN
            );
                break;
            case 2: 
            if (!p->deployables){
                projectile_generic(
                self,
                "Healing",
                healing_aura,
                SHAPE_CIRCLE,
                100,
                0,
                vector2d(-100,-100),
                0.1 * p->strength,
                5,
                p->cldn_skill1,
                self->position,
                think_stationary,
                healingAura_touch,
                NULL
                );
                p->deployables += 1;
            }
                break;
            case 3:
                p->speed = 5;                
                break;
            case 4:
                projectile_generic(
                    self,
                    "Healdart",
                    heal_dart,
                    SHAPE_CIRCLE,
                    8,
                    0,
                    vector2d(-8,-8),
                    15 * p->strength,
                    5,
                    LEVEL_WIDTH/5,
                    self->position,
                    think_move_constVel,
                    heal_dart_touch,
                    NULL                    
                );
                break;
            default: 
                slog("no attack");
        }           
        p->last_skill1 = level_get_active()->frame;
    }
    if (SDL_GameControllerGetButton(p->controller, SDL_CONTROLLER_BUTTON_B) && p->last_skill2 + p->cldn_skill2 < level_get_active()->frame){
        switch (p->index)
        {
            case 1: 
                projectile_generic(
                self,
                "Fireball",
                fireball,
                SHAPE_CIRCLE,
                25,
                0,
                vector2d(-25,-25),
                25 * p->strength,
                5,
                LEVEL_WIDTH/5,
                self->position,
                think_move_constVel,
                fireball_touch,
                NULL
            );
                break;
            case 2: 
            if (!p->deployables){

                vector2d_scale(vScale, self->size, 200);

                vector2d_add(vScaled, self->position, vScale);


                // slog("Player pos: %.2f.%.2f", self->position.x, self->position.y);
                // slog("Proj pos: %.2f.%.2f", vScaled.x, vScaled.y);
                projectile_generic(
                self,
                "Damage Aura",
                damage_aura,
                SHAPE_CIRCLE,
                100,
                0,
                vector2d(-100,-100),
                0.1 * p->strength,
                0,
                p->cldn_skill2,
                vScaled,
                think_stationary,
                damageAura_touch,
                NULL
                );
                p->deployables += 1;
            }
                break;
                case 3:
            if (!p->deployables){
                projectile_generic(
                    self,
                    "Turret",
                    turret,
                    SHAPE_CIRCLE,
                    16,
                    200,
                    vector2d(-16,-16),
                    0.1 * p->strength,
                    0,
                    0,
                    self->position,
                    turret_think,
                    turret_touch,
                    turret_detect
                );
                slog("%s spawned %s", self->name, "turet");
                p->deployables += 1;
            }
                break;
                case 4:
            if (p->deployables <= 2 ){
                projectile_generic(
                    self,
                    "Landmine",
                    landmine,
                    SHAPE_CIRCLE,
                    32,
                    128,
                    vector2d(-32,-32),
                    50*p->strength,
                    0,
                    0,
                    self->position,
                    turret_think,
                    landmine_touch,
                    NULL
                );
                slog("spawned landmine");
                p->deployables += 1;
                break;
            }
            default: 
                slog("no attack for %s index %d", self->name, p->index);
        }   
        p->last_skill2 = level_get_active()->frame;
    }

    if (SDL_GameControllerGetButton(p->controller, SDL_CONTROLLER_BUTTON_X) && p->last_skill3 + p->cldn_skill3 < level_get_active()->frame){

        switch (p->index)
        {
        case 1:
            slog("Companion %s", p->companion->name);
            break;
        case 2:
            hitscan_generic(
                self,
                "Hitscan",
                SHAPE_LINE,
                .1,
                1,
                0,
                rayscan_think,
                hitscan_touch,
                ENT_HITSCAN
            );
            break;
        case 3:
            hitscan_generic(
                self,
                "Hitscan",
                SHAPE_LINE,
                .1,
                2,
                0,
                rayscan_think,
                hitscan_touch,
                ENT_RAYSCAN
            );
            hitscan_generic(
                self,
                "Hitscan",
                SHAPE_LINE,
                .1,
                2,
                10,
                rayscan_think,
                hitscan_touch,
                ENT_RAYSCAN
            );
            hitscan_generic(
                self,
                "Hitscan",
                SHAPE_LINE,
                .1,
                2,
                -10,
                rayscan_think,
                hitscan_touch,
                ENT_RAYSCAN
            );
            break;
        case 4:
            switch (companion->behavior)
                {
                case 1:
                    companion->behavior = 2;
                    p->companion->color = &v4d_blue;
                    break;
                case 2:
                    companion->behavior = 1;
                    p->companion->color = &v4d_yellow;
                    break;
                default:
                    break;
                }
            // slog("Companion %s", p->companion->name);
            break;
        default:
            break;
        }
        p->last_skill3 = level_get_active()->frame;
    }

    if (SDL_GameControllerGetButton(p->controller, SDL_CONTROLLER_BUTTON_Y) && p->last_skill4 + p->cldn_skill4 < level_get_active()->frame){
        vector2d_scale(vScale, self->size, 5);

        switch (p->index)
        {
        case 1:
            vector2d_add(vScaled, self->position, vScale);
            vector2d_copy(self->position, vScaled);
            slog("pos set to %f.%f", self->position.x, self->position.y);
            break;
        case 2:
            vector2d_sub(vScaled, self->position, vScale);
            vector2d_copy(self->position, vScaled);
            slog("pos set to %f.%f", self->position.x, self->position.y);
            break;
        case 3:
            hitscan_generic(
                self,
                "Hitscan",
                SHAPE_LINE,
                10,
                2,
                180,
                rayscan_think,
                hitscan_touch,
                ENT_RAYSCAN
            );            
            break;
        case 4:
            if (companion->last_cldn_1 + companion->cldn_1 < level_get_active()->frame){
                companion->strength = .02;                slog("set to M<EGA");
                companion->last_cldn_1 = level_get_active()->frame;
            }
            break;
        
        default:
            break;
        }
        p->last_skill4 = level_get_active()->frame;
    }

    // slog("Direction of player: %f.%f", p->direction.x, p->direction.y);
    // slog("Axis: %f.%f", x, y);
    // slog("Angle of player %f", p->angle);
    // slog("Latest direction: %f.%f",latestX, latestY);
    // slog("Player %d, Movement vector: %f.%f, direction: %f",p->contNum, p->direction.x, p->direction.y, vector2d_angle(p->direction));
    // slog("Pos: %f.%f",self->position.x, self->position.y);
    // slog("Left stick of %s: %d,%d", SDL_GameControllerName(c),SDL_GameControllerGetAxis(c, SDL_CONTROLLER_AXIS_LEFTX),SDL_GameControllerGetAxis(c, SDL_CONTROLLER_AXIS_LEFTY));  
    
}

void player_touch(Entity *self,Entity *other)
{
    if ((!self) || (!other))return;
    // slog("Player touched thing");
}