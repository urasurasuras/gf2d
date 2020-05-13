#include "companion.h"

Entity *companion_generic(
    Entity *owner_entity,
    TextWord name,
    Sprite *sprite,
    int collider_shape,
    int radius_body,
    int radius_range,
    Vector2D draw_offset,
    Vector2D followOffset,
    float strength,
    int cldn_1,
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
        vector2d_copy(c->followOffset, followOffset);
        c->cldn_1 = cldn_1;
        c->last_cldn_1 = 0;

        self->typeOfEnt = (Companion *)c;
        self->type = ENT_COMPANION;
        self->team = owner_entity->team;
        self->think = think;
        self->touch = touch;
        self->detect = detect;
        return self;
}

Entity* companion_musicBee(Entity* owner_ent)
{
    Entity* bee=
    companion_generic(
        owner_ent,
        "MusicBee",
        sprite_musicBee,
        SHAPE_CIRCLE,
        14,
        100,
        vector2d(-14, -14),
        vector2d(50, -50),
        1,
        0,
        think_musicBee,
        musicBee_touch,
        musicBee_detect
    );

    bee->f_end = 1;

    //bee->drawOffset.x = 0;


    return bee;
}

void think_behavior(Entity *self){
    if (!self)return;
    Companion *c = (Companion *)self->typeOfEnt;
    if (!c->owner_entity)return;

    vector2d_add(self->position, c->owner_entity->position, c->followOffset);
}

void think_musicBee(Entity* self) {
    if (!self)return;
    Companion* c = (Companion*)self->typeOfEnt;
    if (!c->owner_entity)return;

    if (c->owner_entity->size.x < 0)self->flip.x = 1;
    else self->flip.x = 0;

    vector2d_add(self->position, c->owner_entity->position, c->followOffset);

    //Animate
    if (self->f_last + 30 < level_get_active()->frame) {
        self->f_current++;
        self->f_last = level_get_active()->frame;
    }
    if (self->f_current > self->f_end) {
        self->f_current = 0;
    }
}

void lucioAura_touch(Entity *self, Entity *other){
    if (!self || !other)return;
    if (other->team == self->team){
        Companion *c = (Companion *)self->typeOfEnt;
        switch (c->behavior)
        {
        case 1:
            lucio_healingAura_touch(self, other);
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

void lucio_healingAura_touch(Entity *self, Entity *other){
    if (!self)return;
    Companion *p = (Companion *)self->typeOfEnt;
    // Entity *owner_ent = (Entity *)p->owner_entity;

    if (self->team == other->team){
        if (other->type == ENT_PLAYER){
            // Player *other_player = (Player *)other->typeOfEnt;
            other->health += p->strength;
            // slog("Damaged %s %f", other->name, other_player->health);
        }
        else if (other->type == ENT_CORE){
            // Level_core *other_core = (Level_core *)other->typeOfEnt;
            other->health += p->strength;
            // slog("Damaged %s %f", other->name, other_player->health);
        }
    }
}

void speedAura_touch(Entity *self, Entity *other){
    if (!self)return;
    Companion *p = (Companion *)self->typeOfEnt;

    if (self->team == other->team){
        if (other->type == ENT_PLAYER){
            Player *other_player = (Player *)other->typeOfEnt;
            other_player->speed = p->strength*160;
        }
    }
}

void musicBee_touch(Entity *self, Entity *other){
    if (!self)return;
    Companion *p = (Companion *)self->typeOfEnt;
    // Entity *owner_ent = (Entity *)p->owner_entity;

    if (self->team != other->team){
        // slog("%s from team %d x %s from team %d", self->name, self->team, other->name, other->team);
        switch (other->type)
        {
        case ENT_PLAYER:
        case ENT_CORE:
            other->health -= (.2 * p->strength);
            break;
        default:
            break;
        }
    }
}

void musicBee_detect(Entity *self, Entity *other){
    if (!self)return;
    Companion *p = (Companion *)self->typeOfEnt;

    Player* other_player;
    // Entity *owner_ent = (Entity *)p->owner_entity;

    if (other->team != self->team) {
        //Slow down and damaga a bit
        switch (other->type)
        {
        case ENT_PLAYER:
            other_player = (Player*)other->typeOfEnt;
            other_player->speed = 0.5;  //slow down
            other->health -= 0.01;       //sting
        default:
            other->health -= 0.01;       //sting
            break;
        }
    }
    
    //if (self->team != other->team && p->last_cldn_1 + 60 < level_get_active()->frame
    //    && other->type == ENT_PLAYER){

    //    Vector2D v;
    //    vector2d_sub(v, other->position, self->position); 

    //    self->size.x = cos(vector2d_angle(v) * M_PI/180);
    //    self->size.y = sin(vector2d_angle(v) * M_PI/180);

    //    self->rotation.x = -self->drawOffset.x;
    //    self->rotation.y = -self->drawOffset.y;
    //    self->rotation.z = vector2d_angle(vector2d(self->size.x, self->size.y));

    //    // vector2d_copy(self->size, other->position);
    //    // slog("Turret pos %f.%f %s pos %f.%f", self->size.x, self->size.y, other->name, other->position.x, other->position.y);

    //    fireball_projectile(self);

    //    p->last_cldn_1 = level_get_active()->frame;
    //}
}