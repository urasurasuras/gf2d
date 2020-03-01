#include <stdlib.h>
#include "simple_logger.h"
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

void players_spawn(){
    p0 = gf2d_sprite_load_image("images/players/white-circle.png");
    p1 = gf2d_sprite_load_image("images/players/red-circle.png");
    p2 = gf2d_sprite_load_image("images/players/blue-circle.png");
    p3 = gf2d_sprite_load_image("images/players/green-circle.png");

    c0 = SDL_GameControllerOpen(0);
    c1 = SDL_GameControllerOpen(1);
    c2 = SDL_GameControllerOpen(2);
    c3 = SDL_GameControllerOpen(3);

    player_generic("Player1", 1, SHAPE_CIRCLE, 50, vector2d(-50,-50), 1, p0, spawn_top_left, c0, player_think_1, player_touch, 50, 100);
    player_generic("Player2", 2, SHAPE_CIRCLE, 50, vector2d(-50,-50), 1, p1, spawn_top_right, c0, player_think_1, player_touch, 600, 100);
    player_generic("Player3", 3, SHAPE_CIRCLE, 50, vector2d(-50,-50), 1, p2, spawn_bottom_left, c2, player_think_1, player_touch, 50, 100);
    player_generic("Player4", 4, SHAPE_CIRCLE, 50, vector2d(-50,-50), 1, p3, spawn_bottom_right, c3, player_think_1, player_touch, 50, 100);
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
    slog("Player created with index: %d", player->index);
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