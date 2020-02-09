#include <stdlib.h>
#include "simple_logger.h"
#include "entity.h"
#include <SDL.h>


typedef struct 
{
    Uint32 maxEnts;         /**<Maximum number of entities*/
    Entity *entityList;     /**<List of entities*/

}EntityManager;

void PlayerThink (struct Entity_S *self){
    const Uint8 * keys = SDL_GetKeyboardState(NULL); // get the keyboard state for this frame
    // SDL_GameController *controller = NULL;
    //  for (int i = 0; i < SDL_NumJoysticks(); ++i) {
    //     if (SDL_IsGameController(i)) {
    //         controller = SDL_GameControllerOpen(i);
    //         if (controller) {
    //             //slog("Did the thing");
    //             break;
    //         } else {
    //             fprintf(stderr, "Could not open gamecontroller %i: %s\n", i, SDL_GetError());
    //         }
    //     }
    // }  

    // if(keys[SDL_SCANCODE_W] || SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_LEFTY) < 100){
    //     move_up(self);
    // }
    // if(keys[SDL_SCANCODE_S] || SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_LEFTY) > 100){
    //     move_down(self);
    // }
    // if(keys[SDL_SCANCODE_A] || SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_LEFTX) < 100){
    //     move_left(self);
    // }
    // if(keys[SDL_SCANCODE_D] || SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_LEFTX) > 100){
    //     move_right(self);
    // }
    // else if(keys[SDL_SCANCODE_R]){
    //     respawn(self);
    // }
}

static EntityManager entity_manager = {0};

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
    if (!self->think)return;
    self->think(self);
    self->frame = self->frame + 0.1;
    if (self->frame > self->maxFrames)self->frame=0;
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
        self->position,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        (Uint32)self->frame
    );
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
