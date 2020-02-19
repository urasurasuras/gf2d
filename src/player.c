#include <stdlib.h>
#include "simple_logger.h"
#include "entity.h"
#include <SDL.h>
#include "player.h"
#include "level.h"

Vector2D spawn_top_left     = {100, 100};
Vector2D spawn_top_right    = {LEVEL_WIDTH - 100, 100};
Vector2D spawn_bottom_left  = {100, LEVEL_HEIGHT - 100};
Vector2D spawn_bottom_right = {LEVEL_WIDTH - 100, LEVEL_HEIGHT - 100};

void players_spawn(){

    char_new(0,1, "images/white-circle.png", spawn_top_left);
    char_new(1,1, "images/white-circle.png", spawn_top_right);
    char_new(2,1, "images/white-circle.png", spawn_bottom_left);
    char_new(3,1, "images/white-circle.png", spawn_bottom_right);
}

Entity *char_new(int char_index, float default_speed, char sprite_path[], Vector2D init_pos){
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
    self->radius = 50;
    self->size.x = 32;
    self->size.y = 32;
    vector2d_set(self-> drawOffset,-50,-50);
    vector2d_copy(self->position, init_pos);   
    //
    self->think = PlayerThink;
    self->touch = player_touch;
    self->maxFrames = 1;
    self->typeOfEnt = player_new(default_speed,char_index);
    return self;
}

Player *player_new(float speed, int contNum){
    Player *p;
    p = (Player * )malloc(sizeof(Player));
    p->speed = speed;
    p->contNum = contNum;
    p->controller = SDL_GameControllerOpen(contNum);//Returns a gamecontroller identifier 
    return p;
}

void PlayerThink (struct Entity_S *self){
    Player *p = (Player *)self->typeOfEnt;
    SDL_GameController *c = p->controller;

    float x = SDL_GameControllerGetAxis(c, SDL_CONTROLLER_AXIS_LEFTX);
    float y = SDL_GameControllerGetAxis(c, SDL_CONTROLLER_AXIS_LEFTY);

    vector2d_set(p->direction, x, y);

    if (x>DEADZONE || x< (DEADZONE * -1))
    self->position.x += x/ANALOG_SCALE*p->speed;
    if (y>DEADZONE || y< (DEADZONE * -1))
    self->position.y += y/ANALOG_SCALE*p->speed;

    slog("Player %d, Movement vector: %f.%f, direction: %f",p->contNum, p->direction.x, p->direction.y, vector2d_angle(p->direction));

    // slog("Pos: %f.%f",self->position.x, self->position.y);
    // slog("Left stick of %s: %d,%d", SDL_GameControllerName(c),SDL_GameControllerGetAxis(c, SDL_CONTROLLER_AXIS_LEFTX),SDL_GameControllerGetAxis(c, SDL_CONTROLLER_AXIS_LEFTY));  
}

void player_touch(Entity *self,Entity *other)
{
    if ((!self) || (!other))return;
    slog("Player touched thing");
}