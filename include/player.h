#ifndef _PLAYER_H
#define _PLAYER_H
#define DEADZONE 2000
#define ANALOG_SCALE 10000

typedef struct Player_S
{
    float               speed;              /**<Speed of char*/
    int                 contNum;            /**<Index of controller*/
    SDL_GameController *controller;         /**<Pointer to controller*/
}Player;

/**
 * @brief Create player-controlled entity
 * @param char_index Conventional index used for controller index
 * @param default_speed Initial speed multiplier for entity (passed into player params)
 * @param sprite_path File path which leads to spritesheet 
 * @return Entity that contains player object
 * */
Entity *char_new(int char_index, float default_speed, char sprite_path[]);

/**
 * @brief Create object type "Player" which contains controller pointer etc.
 * @param speed Speed multiplier for entity
 * @param contNum Index of assigned controller
 * @return Player object to assign to Entity
 */
Player *player_new(float speed, int contNum);

/**
 * @brief Called every draw update by ent
 * Handles movement from controller input
 * @param self Pointer to entity
 * */
void PlayerThink (struct Entity_S *self);

#endif