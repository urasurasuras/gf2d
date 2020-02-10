#ifndef _PLAYER_H
#define _PLAYER_H

typedef struct Player_S
{
    float       speed;              /**<Speed of char*/
    int         contNum;            /**<Index of controller*/
}Player;

Player *player_new();
void player_manager_init(Uint32 maxEnts);
void player_manager_close();
void player_free(Player *self);
void player_update(Player *self);
void player_update_all();

#endif