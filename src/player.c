#include <stdlib.h>
#include "simple_logger.h"
#include "simple_json.h"
#include "simple_json_value.h"
#include "entity.h"
#include <SDL.h>
#include "player.h"
#include "level.h"
#include "projectile.h"

Vector2D spawn_top_left     = {LEVEL_SPAWN_OFFSET, LEVEL_SPAWN_OFFSET};
Vector2D spawn_top_right    = {LEVEL_WIDTH - LEVEL_SPAWN_OFFSET, LEVEL_SPAWN_OFFSET};
Vector2D spawn_bottom_left  = {LEVEL_SPAWN_OFFSET, LEVEL_HEIGHT - LEVEL_SPAWN_OFFSET};
Vector2D spawn_bottom_right = {LEVEL_WIDTH - LEVEL_SPAWN_OFFSET, LEVEL_HEIGHT - LEVEL_SPAWN_OFFSET};

Sprite *p0;
Sprite *p1;
Sprite *p2;
Sprite *p3;

SDL_GameController *c0;
SDL_GameController *c1;
SDL_GameController *c2;
SDL_GameController *c3;

SJson *saveFile;
SJson *cfgFile;
SJson *arrayOfPlayers;
SJson *arrayOfConf;

void players_spawn(){

    Sprite player_sprite_array[4];

    saveFile = sj_load("data/allData.json");
    cfgFile = sj_load("data/config.json");

    if (!cfgFile){
        slog("no config file, exiting...");
        return;
    }else
    {
        arrayOfConf = sj_object_get_value(cfgFile, "Players");

        int i;
        SJson *player_data;//Data holder per player
        SJson *player_config;//Config holder per player

        SJson *player_name;
        SJson *player_spritePath;
        SJson *player_controller;
        SJson *player_pos;
        SJson *player_health;

        TextWord *name;
        char* spritePath_string;
        int contIndex;
        Vector2D pos;
        float *hp;

        if (!saveFile){
            slog("No savefile...");
        }else
        {
            arrayOfPlayers = sj_object_get_value(saveFile, "Players");
            slog("Loading save file...");
        }
        
        for (i = 0;i < sj_array_get_count(arrayOfConf);i++){
            slog("insde");
            player_config = sj_array_get_nth(arrayOfConf, i);

            player_spritePath = sj_object_get_value(player_config, "Sprite");
            player_controller = sj_object_get_value(player_config, "ControllerIndex");

            if (!saveFile){
                slog("New player");

                player_name = sj_object_get_value(player_config, "Name");
                player_pos = sj_object_get_value(player_config, "Position");
                player_health = sj_object_get_value(player_config, "Health");
            }else
            {
                slog("Existing player");
                player_data = sj_array_get_nth(arrayOfPlayers, i);
                player_name = sj_object_get_value(player_data, "Name");
                player_pos = sj_object_get_value(player_data, "Position");
                player_health = sj_object_get_value(player_data, "Health");
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
                50,
                100
            );

            slog("%s",name);
            sj_echo(player_pos);
            sj_echo(player_health);       
        }
        
            // slog("Found save file ");
            // arrayOfPlayers = sj_object_get_value(saveFile, "Players");
            
            // for (i = 0;i < sj_array_get_count(arrayOfPlayers);i++){
            //     player_data = sj_array_get_nth(arrayOfPlayers, i);
            //     player_config = sj_array_get_nth(arrayOfConf, i);

            //     player_name = sj_object_get_value(player_data, "Name");
            //     player_spritePath = sj_object_get_value(player_config, "Sprite");
            //     player_controller = sj_object_get_value(player_config, "ControllerIndex");
            //     player_pos = sj_object_get_value(player_data, "Position");
            //     player_health = sj_object_get_value(player_data, "Health");

            //     name = (TextWord *)sj_get_string_value(player_name);

            //     SJson *posX;
            //     SJson *posY;

            //     posX = sj_array_get_nth(player_pos, 0);
            //     posY = sj_array_get_nth(player_pos, 1);
            //     sj_get_float_value(posX, &pos.x);
            //     sj_get_float_value(posY, &pos.y);

            //     sj_get_integer_value(player_controller, &contIndex);
            //     spritePath_string = (char *)sj_get_string_value(player_spritePath);

            //     slog("%s",name);
            //     sj_echo(player_pos);
            //     sj_echo(player_health);

            //     player_generic(
            //         (char *)name,
            //         i+1, 
            //         SHAPE_CIRCLE, 
            //         50, 
            //         vector2d(-50,-50), 
            //         1, 
            //         gf2d_sprite_load_image(spritePath_string), 
            //         pos, 
            //         SDL_GameControllerOpen(contIndex), 
            //         player_think_1, 
            //         player_touch, 
            //         50,
            //         100
            //     );
            
        // }
        // else
        // {
        //     sj_save(saveFile, "data/out.json");

        //     // sj_echo(arrayOfPlayers);


        //     p0 = gf2d_sprite_load_image("images/players/white-circle.png");
        //     p1 = gf2d_sprite_load_image("images/players/red-circle.png");
        //     p2 = gf2d_sprite_load_image("images/players/blue-circle.png");
        //     p3 = gf2d_sprite_load_image("images/players/green-circle.png");

        //     c0 = SDL_GameControllerOpen(0);
        //     c1 = SDL_GameControllerOpen(1);
        //     c2 = SDL_GameControllerOpen(2);
        //     c3 = SDL_GameControllerOpen(3);


        //     player_generic("Player1", 1, SHAPE_CIRCLE, 50, vector2d(-50,-50), 1, p0, spawn_top_left, c0, player_think_1, player_touch, 50, 100);
        //     player_generic("Player2", 2, SHAPE_CIRCLE, 50, vector2d(-50,-50), 1, p1, spawn_top_right, c0, player_think_1, player_touch, 600, 100);
        //     player_generic("Player3", 3, SHAPE_CIRCLE, 50, vector2d(-50,-50), 1, p2, spawn_bottom_left, c2, player_think_1, player_touch, 50, 100);
        //     player_generic("Player4", 4, SHAPE_CIRCLE, 50, vector2d(-50,-50), 1, p3, spawn_bottom_right, c3, player_think_1, player_touch, 50, 100);
        // }

    }
    

    
    
   
}

// Player *player_new(float speed, int char_index){
//     Player *p;
//     p = (Player * )malloc(sizeof(Player));
//     p->speed = speed;
//     p->index = char_index;
//     p->health = 100;
//     p->controller = SDL_GameControllerOpen(char_index-1);SDL_GameControllerOpen(char_index-1);//Returns a gamecontroller identifier 
//     p->cldn_skill1 = 50;
//     p->cldn_skill2 = 100;
//     return p;
// }

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
    int cldn_skill2 
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
    self->radius = radius;
    self->size.x = 32;
    self->size.y = 32;
    vector2d_set(self-> drawOffset,-50,-50);
    vector2d_copy(self->position, init_pos);   
    //
    self->think = think;
    self->touch = touch;
    self->maxFrames = 1;

    //Declaration of player
    Player *player;
    player = (Player *)malloc(sizeof(Player));
    //Init player
    player->controller = controller;
    player->cldn_skill1 = cldn_skill1;
    player->cldn_skill2 = cldn_skill2;
    player->health = 100;
    player->index = char_index;
    player->last_skill1 = 0;
    player->last_skill2 = 0;
    // slog("Player created with index: %d", player->index);
    player->speed = default_speed;
    self->typeOfEnt = (Player *)player;
    self->type = ENT_PLAYER;
    return self;
}

void player_think_1 (Entity *self){
    Player *p = (Player *)self->typeOfEnt;
    // SDL_GameController *c = p->controller;

    float x = SDL_GameControllerGetAxis(p->controller, SDL_CONTROLLER_AXIS_LEFTX)/ANALOG_SCALE;
    float y = SDL_GameControllerGetAxis(p->controller, SDL_CONTROLLER_AXIS_LEFTY)/ANALOG_SCALE;
    
    //Direction update
    if (abs(x) < DEADZONE && abs(y) < DEADZONE){
        //If stick is at origin
        //dont update direction
    }else{
        p->direction.x = x;
        p->direction.y = y;
    }
    //Movement
    self->position.x += x*p->speed;
    self->position.y += y*p->speed;
    if (level_bounds_test_circle(level_get_active(), self->position, self->radius))
    {
        //TODO: Do something is ent hits bounds
        self->position.x -= x*p->speed;
        self->position.y -= y*p->speed;
    }

    if (SDL_GameControllerGetButton(p->controller, SDL_CONTROLLER_BUTTON_A) && p->last_skill1 + p->cldn_skill1 < level_get_active()->frame){
        // slog("Char: %d",p->index);
        switch (p->index)
        {
            case 1: 
                projectile_generic(
                self,
                "Fireball",
                fireball,
                SHAPE_CIRCLE,
                25,
                vector2d(-25,-25),
                10,
                10,
                fireball_think,
                fireball_touch
            );
            // slog("case1");
                break;
            case 2: 
                projectile_generic(
                self,
                "Healing",
                healing,
                SHAPE_CIRCLE,
                100,
                vector2d(-100,-100),
                10,
                5,
                healingAura_think,
                healingAura_touch
            );
            // slog("case2");
                break;
            default: 
                slog("no attack");
// code to be executed if n doesn't match any cases
        }   
        
        
        p->last_skill1 = level_get_active()->frame;
        // slog("Dir: %f.%f", p->direction.x, p->direction.y);
        // slog("Last used after set: %d", last_s1);      
        // slog("got a");
    }
    if (SDL_GameControllerGetButton(p->controller, SDL_CONTROLLER_BUTTON_B) && p->last_skill2 + p->cldn_skill2 < level_get_active()->frame){
        projectile_generic(
            self,
            "Fireball",
            fireball,
            SHAPE_CIRCLE,
            25,
            vector2d(-25,-25),
            25,
            3,
            fireball_think,
            fireball_touch
        );
        p->last_skill2 = level_get_active()->frame;
        slog("got b");
    }

    if (p->health <= 0){
        entity_free(self);
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