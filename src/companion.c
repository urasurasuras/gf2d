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

void think_behavior(Entity *self){
    if (!self)return;
    Companion *c = (Companion *)self->typeOfEnt;
    if (!c->owner_entity)return;

    vector2d_add(self->position, c->owner_entity->position, c->followOffset);
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
        if (other->type == ENT_PLAYER){
            // Player *other_player = (Player *)other->typeOfEnt;
            other->health -= p->strength;
            // slog("Damaged %s %f", other->name, other_player->health);
        }
        else if (other->type == ENT_CORE){
            // Level_core *other_core = (Level_core *)other->typeOfEnt;
            other->health -= p->strength;
            // slog("Damaged %s %f", other->name, other_player->health);
        }
    }
}

void musicBee_detect(Entity *self, Entity *other){
    if (!self)return;
    Companion *p = (Companion *)self->typeOfEnt;
    // Entity *owner_ent = (Entity *)p->owner_entity;

    if (self->team != other->team && p->last_cldn_1 + 60 < level_get_active()->frame
        && other->type == ENT_PLAYER){

        Vector2D v;
        vector2d_sub(v, other->position, self->position); 

        self->size.x = cos(vector2d_angle(v) * M_PI/180);
        self->size.y = sin(vector2d_angle(v) * M_PI/180);
        // vector2d_copy(self->size, other->position);
        // slog("Turret pos %f.%f %s pos %f.%f", self->size.x, self->size.y, other->name, other->position.x, other->position.y);

        projectile_generic(
            self,
            "BeeNote",
            fireball,
            28,
            SHAPE_CIRCLE,
            25,
            0,
            25 * p->strength,
            3,
            LEVEL_WIDTH/3,
            self->position,
            think_move_constVel,
            fireball_touch,
            NULL
        ); 

        p->last_cldn_1 = level_get_active()->frame;
    }
}