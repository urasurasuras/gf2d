#include "entity.h"
#include "level.h"
#include "player.h"

static EntityManager entity_manager = {0};

EntityManager *entity_manager_get_active()
{
    return &entity_manager;
}

Entity *entity_new(){
    int i;
    for (i=0; i < entity_manager.maxEnts; i++){
        if(entity_manager.entityList[i]._inuse)continue;
        entity_manager.entityList[i]._inuse = 1;
        entity_manager.entityList[i].f_current = 0;
        entity_manager.entityList[i].f_end = 0;
        entity_manager.entityList[i].f_last = 0;
        entity_manager.entityList[i].drawhp = 0;
        entity_manager.entityList[i].size = vector2d(0, 0);
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
        slog("TTF_Init: %s\n", TTF_GetError());
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
    //if ()
    if (self->touch){
        entity_collision_check(self);
        if (self->type == ENT_RAYSCAN){
            // slog("this is rayscan");
            // Projectile *p = (Projectile *)self->typeOfEnt;
            //     if (p->firstContact){
            //         self->touch(self, p->firstContact);
            //     }
            }
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
    int_Vector2D hit_edges;

    for (i = 0;i < entity_manager.maxEnts;i++)
    {
        if (!entity_manager.entityList[i]._inuse)continue;

        hit_edges = level_bounds_test_circle(level_get_active()->bounds_level, entity_manager.entityList[i].position, entity_manager.entityList[i].radius_body);
        if (entity_manager.entityList[i].bound_hit/* && hit_edge != null*/) {
            //if (entity_manager.entityList[i].type == ENT_NEUTRAL_MONSTER) {
            //    slog("Monster %s hit edge %d", entity_manager.entityList[i].name, hit_edges.x);
            //}
            entity_manager.entityList[i].bound_hit(&entity_manager.entityList[i], hit_edges);
        }
        entity_update(&entity_manager.entityList[i]);

    }
    //Update map
}

void entity_draw(Entity *self){
    if (self == NULL){
        slog("cannot draw, null entity provided");
        return;
    }
    // Vector2D scaler = vector2d(0.7,0.5);
    // Vector2D *scalerPtr = &scaler;
    
    if (self->sprite){
        gf2d_sprite_draw(
            self->sprite,
            vector2d(self->position.x + self->drawOffset.x, self->position.y + self->drawOffset.y),
            NULL,
            NULL,
            &self->rotation,
            NULL,
            self->color,
            (Uint32)self->f_current
        );
    }
    if (self->drawhp/* && self->type == ENT_PLAYER*/){
        char hp[8];
        snprintf(hp, 8, "%.0f", self->health);
        SDL_Surface* surfaceMessage = TTF_RenderText_Solid(entity_manager_get_active()->font, hp, entity_manager_get_active()->font_color); // as TTF_RenderText_Solid could only be used on SDL_Surface then you have to create the surface first
        SDL_Texture* Message = SDL_CreateTextureFromSurface(gf2d_graphics_get_renderer(), surfaceMessage); //now you can convert it into a texture
        SDL_RenderCopy(
        gf2d_graphics_get_renderer(), 
        Message, NULL, &self->ui_box); //you put the renderer's name first, the Message, the crop size(you can ignore this if you don't want to dabble with cropping), and the rect which is the size and coordinate of your texture
        SDL_DestroyTexture(Message);
        SDL_FreeSurface(surfaceMessage);
    }
    //Vector4D color;
    //gf2d_draw_rect(self->ui_box, vector4d(255, 155, 255, 255));

    if (self->collider_shape == SHAPE_LINE){
        gf2d_draw_line(self->position, self->size, vector4d(255,0,255,255));
    }
    //draw circle collider
    else if (self->collider_shape == SHAPE_CIRCLE){
        gf2d_draw_circle(self->position, self->radius_body, vector4d(255,0,255,255));    
    }
    //draw range
    if (self->radius_range){
        gf2d_draw_circle(self->position, self->radius_range, vector4d(255,255,255,255));    
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
    e2->lastHit = e1->team;
    //slog("%s was hit by %d", e2->name, e2->lastHit);
    if (e1->radius_range && e1->detect){
        if (collide_circle(e1->position, e1->radius_range, e2->position, e2->radius_body))
        {//Range to circle
            e1->detect(e1,e2);
        }
    }
    if (e1->collider_shape == SHAPE_CIRCLE){
        if (collide_circle(e1->position, e1->radius_body, e2->position, e2->radius_body))
        {//Circle-to-circle
            if (e1->touch)
            {
                e1->touch(e1,e2);
            }
        }
    }
    if (e1->collider_shape == SHAPE_RECT){
        //TODO: Add check for rect collision
    }
    if (e1->collider_shape == SHAPE_LINE){
        Projectile *h = (Projectile *)e1->typeOfEnt;
        if (!h)return;
        if (e2 != h->owner_entity){
            if (e2->collider_shape == SHAPE_CIRCLE && e2->type == ENT_PLAYER){

                if (lineCircle(e1->position, e1->size, e2->position, e2->radius_body))
                {//Line to circle
                    // slog("%s hit %s ", e1->name, e2->name);
                    if (e1->type == ENT_RAYSCAN){

                        if (h->firstContact == NULL){
                            slog("fresh collision %s",e2->name);

                            h->firstContact = e2;
                        }
                        else if (h->firstContact)
                        {
                            // slog("%d", h->firstContact->team);
                            if (fabsf((e1->position.x) - e2->position.x) < fabsf((e1->position.x) - h->firstContact->position.x)    &&
                            fabsf((e1->position.y) - e2->position.y) < fabsf((e1->position.y) - h->firstContact->position.y)){
                                slog("new collision %f",fabsf((e1->position.x) - h->firstContact->position.x));
                                h->firstContact = e2;
                            }
                        }    
                    }
                    else if (e1->type == ENT_HITSCAN){
                        
                        if (e1->touch)
                        {
                            e1->touch(e1,e2);
                        }                    
                    }
                }
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