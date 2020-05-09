#ifndef _PRELOADER_H_
#define _PRELOADER_H_
#include <game.h>
#include "gf2d_sprite.h"

/**
*   Sprites
*/

//Blank sprite
Sprite* blank;
//Background sprites
Sprite* bg_flat;
Sprite* bg_synth;
//Projectile sprites
Sprite* fireball;
Sprite* healing_aura;
Sprite* damage_aura;
Sprite* turret;
Sprite* heal_dart;
Sprite* landmine;
Sprite* lucio_aura;
//Pickup sprites
Sprite* pickup_health;
Sprite* pickup_boost;
Sprite* pickup_speed;
//Companion sprites
Sprite* companion_musicBee;

/**
*   Sounds
*/

Sound* main_theme;
Sound* lava_theme;
Sound* grass_theme;

void load_sprites();

/**
 * @brief Pre-loads projectile sprites into individual pointers for later use
 * */
void load_sprites_projectiles();

void load_sounds();
#endif // !_PRELOADER_H_
