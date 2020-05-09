#include "PRELOADER.h"

void load_sprites() {
    blank = gf2d_sprite_load_image("images/empty.png");
    bg_flat = gf2d_sprite_load_image("images/backgrounds/bg_flat.png");
    bg_synth = gf2d_sprite_load_image("images/backgrounds/bg_blue.png");
}
void load_sprites_projectiles() {
    //fireball = gf2d_sprite_load_image("images/projectiles/fireball.png");
    fireball = gf2d_sprite_load_all("images/projectiles/Fireball/Fireball-idle.png", 64, 64, 4);
    healing_aura = gf2d_sprite_load_image("images/projectiles/healing_aura.png");
    damage_aura = gf2d_sprite_load_image("images/projectiles/damage_aura.png");
    turret = gf2d_sprite_load_image("images/projectiles/turret.png");
    heal_dart = gf2d_sprite_load_image("images/projectiles/heal_dart.png");
    landmine = gf2d_sprite_load_image("images/projectiles/landmine.png");
    lucio_aura = gf2d_sprite_load_image("images/projectiles/aura_generic_large.png");

    pickup_health = gf2d_sprite_load_image("images/pickups/pickup_health.png");
    pickup_boost = gf2d_sprite_load_image("images/pickups/pickup_boost.png");
    pickup_speed = gf2d_sprite_load_image("images/pickups/pickup_speed.png");

    companion_musicBee = gf2d_sprite_load_all("images/companions/music_bee.png", 28, 28, 2);
    // companion_musicBee = gf2d_sprite_load_image("images/companions/music_bee.png");
}