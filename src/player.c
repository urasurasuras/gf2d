#include <stdlib.h>
#include "simple_logger.h"
#include "entity.h"
#include <SDL.h>
#include "player.h"

Entity *char_new(int char_index, float default_speed, char sprite_path[]){
    Entity *self;
    self = entity_new();
    if (!self)return NULL;
    self->sprite = gf2d_sprite_load_all(
        sprite_path,
        100,
        100,
        1
    );
    self->think = PlayerThink;
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

    if (x>DEADZONE || x< (DEADZONE * -1))
    self->position.x += x/ANALOG_SCALE*p->speed;
    if (y>DEADZONE || y< (DEADZONE * -1))
    self->position.y += y/ANALOG_SCALE*p->speed;

    // slog("Pos: %f.%f",self->position.x, self->position.y);
    // slog("Left stick of %s: %d,%d", SDL_GameControllerName(c),SDL_GameControllerGetAxis(c, SDL_CONTROLLER_AXIS_LEFTX),SDL_GameControllerGetAxis(c, SDL_CONTROLLER_AXIS_LEFTY));  
}