#include "level_core.h"
#include "level.h"

Level_core *level_core_new(Sprite *sprite){
    Level_core *core;
    core = (Level_core *)malloc(sizeof(Level_core));
    core->sprite = sprite;
    core->position = vector2d(LEVEL_WIDTH/2, LEVEL_HEIGHT/2);
    core->health = 500;
    core->radius = 75;
    core->drawOffset.x = core->position.x - core->radius;
    core->drawOffset.y = core->position.y - core->radius;
    return core;
}

void level_core_think();

void level_core_touch();

void level_core_draw(){
    if (!level_get_active()->core)return;
    gf2d_sprite_draw_image(level_get_active()->core->sprite, level_get_active()->core->drawOffset);
}