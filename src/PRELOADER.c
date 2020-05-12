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
    sprite_turret = gf2d_sprite_load_image("images/projectiles/turret.png");
    heal_dart = gf2d_sprite_load_all("images/projectiles/heal_dart_sheet.png",16,16,4);
    landmine = gf2d_sprite_load_image("images/projectiles/landmine.png");
    lucio_aura = gf2d_sprite_load_image("images/projectiles/aura_generic_large.png");

    pickup_health = gf2d_sprite_load_image("images/pickups/pickup_health.png");
    pickup_boost = gf2d_sprite_load_image("images/pickups/pickup_boost.png");
    pickup_speed = gf2d_sprite_load_image("images/pickups/pickup_speed.png");

    sprite_musicBee = gf2d_sprite_load_all("images/companions/music_bee.png", 28, 28, 2);
    // companion_musicBee = gf2d_sprite_load_image("images/companions/music_bee.png");
}
void load_sprites_chars()
{
    sprite_player1 = gf2d_sprite_load_image("images/players/white-circle.png");
    sprite_player2 = gf2d_sprite_load_image("images/players/red-circle.png");
    sprite_player3 = gf2d_sprite_load_image("images/players/blue-circle.png");
    sprite_player4 = gf2d_sprite_load_image("images/players/green-circle.png");
}
void load_sounds() {
    main_theme = gfc_sound_load("audio/njit-theme-song.mp3", .1, 0);
    lava_theme = gfc_sound_load("audio/agl-bardock-ost-extended.mp3", .1, 1);
    grass_theme = gfc_sound_load("audio/21-dan-14-tsumi-dan-14-zui.mp3", .1, 2);
    sound_fireball = gfc_sound_load("audio/fireball.wav", .1, 3);
}