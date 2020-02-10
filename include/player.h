#ifndef _PLAYER_H
#define _PLAYER_H

typedef struct Player_S
{
    float               speed;              /**<Speed of char*/
    int                 contNum;            /**<Index of controller*/
    SDL_GameController *controller;         /**<Pointer to controller*/
}Player;

/**
 * @brief Player think function
 * @param self Pointer to entity
 * */
void PlayerThink (struct Entity_S *self);

Player *player_new(float speed, int contNum);
void player_manager_init(Uint32 maxEnts);
void player_manager_close();
void player_free(Player *self);
void player_update(Player *self);
void player_update_all();

#endif