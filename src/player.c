#include <stdlib.h>
#include "simple_logger.h"
#include "entity.h"
#include <SDL.h>
#include "player.h"

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

    // if(SDL_GameControllerGetAxis(c, SDL_CONTROLLER_AXIS_LEFTY) < 1000){
    //     self->position.y -= p->speed;
    // }
    // if(SDL_GameControllerGetAxis(c, SDL_CONTROLLER_AXIS_LEFTY) > 1000){
    //     self->position.y += p->speed;
    // }
    // if(SDL_GameControllerGetAxis(c, SDL_CONTROLLER_AXIS_LEFTX) < 1000){
    //     self->position.x -= p->speed;
    // }
    // if(SDL_GameControllerGetAxis(c, SDL_CONTROLLER_AXIS_LEFTX) > 1000){
    //     self->position.x += p->speed;
    // }
    slog("Pos: %f.%f",self->position.x, self->position.y);
    //slog("Left stick of %s: %d,%d", SDL_GameControllerName(c),SDL_GameControllerGetAxis(c, SDL_CONTROLLER_AXIS_LEFTX),SDL_GameControllerGetAxis(c, SDL_CONTROLLER_AXIS_LEFTY));  
}