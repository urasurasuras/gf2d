#include "level_core.h"
#include "level.h"

Entity *level_core_new(Sprite *sprite, int team){
    Entity *core_ent = entity_new();
    strcpy(core_ent->name, "Grass Core");
    Level_core *core;
    core = (Level_core *)malloc(sizeof(Level_core));
    core_ent->sprite = sprite;
    core_ent->position = vector2d(LEVEL_WIDTH/2, LEVEL_HEIGHT/2);
    slog("%s location: %f,%f", core_ent->name, core_ent->position.x, core_ent->position.y);
    core_ent->health = 500;
    core_ent->radius = 75;
    vector2d_set(core_ent->drawOffset,-core_ent->radius,-core_ent->radius);
    // core_ent->drawOffset.x = core_ent->position.x - core_ent->radius;
    // core_ent->drawOffset.y = core_ent->position.y - core_ent->radius;
    slog("%s draw: %f,%f", core_ent->name, core_ent->drawOffset.x, core_ent->drawOffset.y);
    core_ent->team = team;
    switch (team)
    {
    case TEAM_A:
        slog("Created core for team A");
        core_ent->color = &v4d_blue;
        core_ent->position.x -= LEVEL_WIDTH/4;
        break;
    case TEAM_B:
        core_ent->color = &v4d_red;
        core_ent->position.x += LEVEL_WIDTH/4;
        break;
    default:
        break;
    }
    core_ent->type = ENT_CORE;
    core_ent->typeOfEnt = (Level_core *)core;

    return core_ent;
}

void level_core_think(Entity *self){
    if(!self)return;
    Level_core *core = (Level_core *)self->typeOfEnt;
    if (self->health <= 0){
        self->_inuse = 0;
        //TODO: win condition
    }
    // int i;
    // for (i = 0;i < entity_manager_get_active().maxEnts;i++)
    // {
    //     if (!entity_manager_get_active().entityList[i]._inuse)continue;
    //     if (entity_manager_get_active().entityList[i].type == ENT_PROJECTILE){
    //         level_core_touch(self, &entity_manager_get_active().entityList[i]);
    //     }
    //     entity_update(&entity_manager_get_active().entityList[i]);
    // }
    
}

void level_core_touch(Entity *self, Entity *other){
    // if (!self)return;
    // if (!other)return;
    // if (other->type == ENT_PROJECTILE){
    //     self->health -= 
    // }
}

// void level_core_draw(){
//     if (!level_get_active()->core)return;
//     gf2d_sprite_draw_image(level_get_active()->core->sprite, level_get_active()->core->drawOffset);
// }