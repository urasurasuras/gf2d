#include "entity.h"
#include "level.h"
#include "player.h"

static EntityManager entity_manager = {0};

EntityManager entity_manager_get_active()
{
    return entity_manager;
}

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
    if(TTF_Init()==-1) {
        printf("TTF_Init: %s\n", TTF_GetError());
        exit(2);
    }
    entity_manager.font = TTF_OpenFont("fonts/bignoodletoo.ttf", 72); //this opens a font style and sets a size
    if (!entity_manager.font)slog("no font");
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
    slog("freed ent: %s", self->name);
    gf2d_sprite_free(self->sprite);
    memset(self,0,sizeof(Entity));
}

void entity_update(Entity *self){
    if (!self)return;
    if (self->think){
        self->think(self);
    }
    if (self->touch){
        entity_collision_check(self);
    }else if (!self->touch && self->name){
        // slog("No touch func for %s", self->name);
    }
    //TODO: anim
    // self->frame = self->frame + 0.1;
    // if (self->frame > self->maxFrames)self->frame=0;

}

void entity_update_all(){
    level_get_active()->frame ++;
    int i;
    for (i = 0;i < entity_manager.maxEnts;i++)
    {
        if (!entity_manager.entityList[i]._inuse)continue;
        entity_update(&entity_manager.entityList[i]);
    }
    //Update map
    static int num_pickups;
    

}

void entity_draw(Entity *self){
    if (self == NULL){
        slog("cannot draw, null entity provided");
        return;
    }
    // Vector2D scaler = vector2d(0.7,0.5);
    // Vector2D *scalerPtr = &scaler;
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

    if (self->type == ENT_PLAYER){
        Player *p = (Player *)self->typeOfEnt;

        char hp[16];
        snprintf(hp,16, "%.2f", p->health);
        SDL_Surface* surfaceMessage = TTF_RenderText_Solid(entity_manager_get_active().font, &hp, entity_manager_get_active().font_color); // as TTF_RenderText_Solid could only be used on SDL_Surface then you have to create the surface first
        SDL_Texture* Message = SDL_CreateTextureFromSurface(gf2d_graphics_get_renderer(), surfaceMessage); //now you can convert it into a texture
        SDL_RenderCopy(
        gf2d_graphics_get_renderer(), 
        Message, NULL, &self->ui_box); //you put the renderer's name first, the Message, the crop size(you can ignore this if you don't want to dabble with cropping), and the rect which is the size and coordinate of your texture
    }

    else if (self->collider_shape == SHAPE_LINE){
        gf2d_draw_line(self->position, self->size, vector4d(255,0,255,255));
    }
    //draw circle collider
    else if (self->collider_shape == SHAPE_CIRCLE){
        gf2d_draw_circle(self->position, self->radius, vector4d(255,0,255,255));    
    }
    
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

void entity_entity_collide(Entity *e1, Entity *e2)
{
    // if (e1->name)
    // slog("Checking collision for %s", e1->name);
    if (e1->collider_shape == SHAPE_CIRCLE){
        // slog("Shape cictle");
        if (collide_circle(e1->position, e1->radius, e2->position, e2->radius))
        {//Circle-to-circle
            // slog("Collision check true");
            if (e1->touch)
            {
                // slog("touch function exists");
                e1->touch(e1,e2);
            }
        }
    }
    if (e1->collider_shape == SHAPE_RECT){
        //TODO: Add check for rect collision
    }
    if (e1->collider_shape == SHAPE_LINE && e1->type == ENT_HITSCAN){
        // slog("this ent is hitscan");
        if (e2->collider_shape == SHAPE_CIRCLE && e2->type == ENT_PLAYER){
            // slog("other ent is player");
            if (lineCircle(e1->position, e1->size, e2->position, e2->radius)){
                if (e1->touch)
                {
                    e1->touch(e1,e2);
                }
            // slog("%s hit %s", e1->name, e2->name);
            }
        }
    }
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