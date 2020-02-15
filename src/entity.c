#include <stdlib.h>
#include "simple_logger.h"
#include "entity.h"
#include <SDL.h>
#include "gf2d_draw.h"
#include "collision.h"
#include "level.h"

typedef struct 
{
    Uint32 maxEnts;         /**<Maximum number of entities*/
    Entity *entityList;     /**<List of entities*/

}EntityManager;

static EntityManager entity_manager = {0};

// void entity_collision_check(Entity *entity);

Entity *entity_new(){
    int i;
    for (i=0; i < entity_manager.maxEnts; i++){
        if(entity_manager.entityList[i]._inuse)continue;
        entity_manager.entityList[i]._inuse = 1;
        //slog("Items in ent list %d", i);
        return &entity_manager.entityList[i];
    }
    slog("out of open entity slots in memory");
    return NULL;
}

void entity_manager_init(Uint32 maxEnts){
    if (entity_manager.entityList != NULL){
        //TODO: cleanup
    }
    if (!maxEnts){
        slog("cannot init 0 zise net list");
        return;
    }
    entity_manager.entityList = (Entity * )malloc(sizeof(Entity) * maxEnts);
    if (entity_manager.entityList == NULL){
        slog("failed to allocate %i entites for the ent manager",maxEnts);
    }
    entity_manager.maxEnts = maxEnts;
    memset(entity_manager.entityList,0,sizeof(Entity)*maxEnts);
    slog("entity manager initalized");
    atexit(entity_manager_close);
}

void entity_manager_close(){
    int i;
    for (i=0; i < entity_manager.maxEnts; i++){
        if(entity_manager.entityList[i]._inuse){
            entity_free(&entity_manager.entityList[i]);
        }
    }
    entity_manager.maxEnts = 0;
    free(entity_manager.entityList);
    entity_manager.entityList =NULL;
    slog("entity manager closed");
}

void entity_free(Entity *self){
    if (!self)return;
    gf2d_sprite_free(self->sprite);
    memset(self,0,sizeof(Entity));
}

void entity_update(Entity *self){
    if (!self)return;
    if (self->think){
        self->think(self);
    }
    self->frame = self->frame + 0.1;
    if (self->frame > self->maxFrames)self->frame=0;
    if (level_bounds_test_circle(level_get_active(), self->position, self->radius))
    {slog("Hit bounds");}
    entity_collision_check(self);
}

void entity_update_all(){
    int i;
    for (i = 0;i < entity_manager.maxEnts;i++)
    {
        if (!entity_manager.entityList[i]._inuse)continue;
        entity_update(&entity_manager.entityList[i]);
    }
}

void entity_draw(Entity *self){
    if (self == NULL){
        slog("cannot draw, null entity provided");
        return;
    }
    gf2d_sprite_draw(
        self->sprite,
        vector2d(self->position.x + self->drawOffset.x,self->position.y + self->drawOffset.y),
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        (Uint32)self->frame
    );
    //draw circle collider
    gf2d_draw_circle(self->position, self->radius, vector4d(255,0,255,255));

}

void entity_draw_all()
{
    int i;
    for (i = 0;i < entity_manager.maxEnts;i++)
    {
        if (!entity_manager.entityList[i]._inuse)continue;
        entity_draw(&entity_manager.entityList[i]);
    }
}

void entity_entity_collide(Entity *e1,Entity *e2)
{
    if (collide_circle(e1->position, e1->radius, e2->position, e2->radius))
    {
        if (e1->touch)
        {
            e1->touch(e1,e2);
        }
    }
    //TODO: Add check for rect collision
}
void entity_collision_check(Entity *entity)
{
    int i;
    if (!entity)return;
    for (i = 0;i < entity_manager.maxEnts;i++)
    {    

        if (!entity_manager.entityList[i]._inuse)continue;
        if (&entity_manager.entityList[i] == entity)continue;
        
        entity_entity_collide(entity,&entity_manager.entityList[i]);
    }
}