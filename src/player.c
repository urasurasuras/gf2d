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

void players_spawn(){

    player_new_ent(0,1, "images/players/white-circle.png", spawn_top_left);
    player_new_ent(1,1, "images/players/red-circle.png", spawn_top_right);
    player_new_ent(2,1, "images/players/blue-circle.png", spawn_bottom_left);
    player_new_ent(3,1, "images/players/green-circle.png", spawn_bottom_right);
}

Player *player_new(float speed, int contNum){
    Player *p;
    p = (Player * )malloc(sizeof(Player));
    p->speed = speed;
    p->contNum = contNum;
    p->health = 100;
    p->controller = SDL_GameControllerOpen(contNum);//Returns a gamecontroller identifier 
    p->cldn_skill1 = 50;
    p->cldn_skill2 = 100;
    return p;
}

Entity *player_new_ent(int char_index, float default_speed, char sprite_path[], Vector2D init_pos){
    Entity *self;
    self = entity_new();
    if (!self)return NULL;
    self->sprite = gf2d_sprite_load_all(
        sprite_path,
        100,
        100,
        1
    );
    //
    self->collider_shape = SHAPE_CIRCLE;
    self->radius = 50;
    self->size.x = 32;
    self->size.y = 32;
    vector2d_set(self-> drawOffset,-50,-50);
    vector2d_copy(self->position, init_pos);   
    //
    self->think = player_think_1;
    self->touch = player_touch;
    self->maxFrames = 1;
    self->typeOfEnt = player_new(default_speed,char_index);
    return self;
}

void player_think_1 (Entity *self){
    Player *p = (Player *)self->typeOfEnt;
    SDL_GameController *c = p->controller;
    static int last_s1 = 0;
    static int last_s2 = 0;

    float x = SDL_GameControllerGetAxis(c, SDL_CONTROLLER_AXIS_LEFTX)/ANALOG_SCALE;
    float y = SDL_GameControllerGetAxis(c, SDL_CONTROLLER_AXIS_LEFTY)/ANALOG_SCALE;
    
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

    
    // slog("at frame %d", entity_manager.frame);
    // slog("Last used s1 at frame: %d", last_s1);
    if (SDL_GameControllerGetButton(c,SDL_CONTROLLER_BUTTON_A) && last_s1 + p->cldn_skill1 < level_get_active()->frame){
        projectile_generic(
            self,
            "Fireball",
            SHAPE_CIRCLE,
            25,
            vector2d(-25,-25),
            10,
            10
        );
        last_s1 = level_get_active()->frame;
        // slog("Dir: %f.%f", p->direction.x, p->direction.y);
        // slog("Last used after set: %d", last_s1);      
        // slog("got a");
    }
    if (SDL_GameControllerGetButton(c,SDL_CONTROLLER_BUTTON_B) && last_s2 + p->cldn_skill2 < level_get_active()->frame){
        projectile_generic(
            self,
            "Fireball",
            SHAPE_CIRCLE,
            25,
            vector2d(-25,-25),
            25,
            3
        );
        last_s2 = level_get_active()->frame;
        slog("Last used after set: %d", last_s2);      
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