#include "companion.h"

Entity *companion_generic(
    Entity *owner_entity,
    TextWord name,
    Sprite *sprite,
    int collider_shape,
    int radius_body,
    int radius_range,
    Vector2D draw_offset,
    float strength,
    void (*think)(struct Entity_S *self),
    void (*touch)(struct Entity_S *self, struct Entity_S *other),
    void (*detect)(struct Entity_S *self, struct Entity_S *other)
){
    Entity *self;
        self = entity_new();
        if (!self)return NULL;
        //Init ent
        strcpy(self->name, name);
        self->sprite = sprite;
        self->collider_shape = collider_shape;
        if (collider_shape == SHAPE_CIRCLE)self->radius_body = radius_body;
        self->drawOffset = draw_offset;   
        self->radius_body = radius_body;  
        self->radius_range = radius_range;   

        //Declaration of companion
        Companion *c;
        c = (Companion * )malloc(sizeof(Companion));
        //Initialization of companion
        c->owner_entity = owner_entity;
        // vector2d_copy(self->size, owner_entity->size);
        // c->speed = speed;
        c->time_alive = 0;
        c->strength = strength;
        c->behavior = 1;
        self->typeOfEnt = (Companion *)c;
        self->type = ENT_COMPANION;
        self->team = owner_entity->team;
        self->think = think;
        self->touch = touch;
        self->detect = detect;
        return self;
}

void think_behavior(Entity *self){
    if (!self)return;
    Companion *c = (Companion *)self->typeOfEnt;
    if (!c->owner_entity)return;

    Player *p = (Player *)c->owner_entity->typeOfEnt;
    SDL_GameController *controller = p->controller;

    if (SDL_GameControllerGetButton(p->controller, SDL_CONTROLLER_BUTTON_X) && p->last_skill3 + p->cldn_skill3 < level_get_active()->frame){
        switch (c->behavior)
        {
        case 1:
            c->behavior = 2;
            self->color = &v4d_blue;
            break;
        case 2:
            c->behavior = 1;
            self->color = &v4d_yellow;
            break;
        default:
            break;
        }
        p->last_skill3 = level_get_active()->frame;
    }
    
    vector2d_copy(self->position, c->owner_entity->position);
}

void lucioAura_touch(Entity *self, Entity *other){
    if (!self || !other)return;
    if (other->team == self->team){
        Companion *c = (Companion *)self->typeOfEnt;
        switch (c->behavior)
        {
        case 1:
            healingAura_touch(self, other);
            // slog("%s heailng %s", self->name, other->name);
            break;
        case 2:
            speedAura_touch(self, other);
            // slog("%s speeding %s", self->name, other->name);
            break;
        default:
            break;
        }
    }
}