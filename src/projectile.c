#include "projectile.h"

void projectile_load_sprites(){

    blank = gf2d_sprite_load_image("images/empty.png");
    fireball = gf2d_sprite_load_image("images/projectiles/fireball.png");
    healing_aura = gf2d_sprite_load_image("images/projectiles/healing_aura.png");
    damage_aura = gf2d_sprite_load_image("images/projectiles/damage_aura.png");
    turret = gf2d_sprite_load_image("images/projectiles/turret.png");
    heal_dart = gf2d_sprite_load_image("images/projectiles/heal_dart.png");
    landmine = gf2d_sprite_load_image("images/projectiles/landmine.png");
    lucio_aura = gf2d_sprite_load_image("images/projectiles/aura_generic_large.png");

    pickup_health = gf2d_sprite_load_image("images/pickups/pickup_health.png");
    pickup_boost = gf2d_sprite_load_image("images/pickups/pickup_boost.png");
    pickup_speed = gf2d_sprite_load_image("images/pickups/pickup_speed.png");

    companion_musicBee = gf2d_sprite_load_all("images/companions/music_bee.png", 28, 28, 2);
    // companion_musicBee = gf2d_sprite_load_image("images/companions/music_bee.png");

    // SJson *config = level_get_active()->config;

    // SJson *projectile_array_config = sj_object_get_value(config, "Projectiles");
    // int i;
    // for (i = 0;i < sj_array_get_count(projectile_array_config);i++){
    //     SJson *config_proj_data = sj_array_get_nth(projectile_array_config, i);

    //     SJson *projectile_name = sj_object_get_value(config_proj_data, "Name");
    //     SJson *projectile_spritePath = sj_object_get_value(config_proj_data, "Sprite");
    //     char *name_string = (char *)sj_get_string_value(projectile_name);
    //     char *spritePath_string = (char *)sj_get_string_value(projectile_spritePath);
    //     slog("Loaded sprite %s", name_string);

    //     if (!strcmp(name_string, "projectile_fireball")){
    //         fireball = gf2d_sprite_load_image(spritePath_string);
    //     }
    //     else if (!strcmp(name_string, "projectile_healingAura")){
    //         healing_aura = gf2d_sprite_load_image(spritePath_string);
    //     }
    //     else if (!strcmp(name_string, "projectile_damageAura")){
    //         damage_aura = gf2d_sprite_load_image(spritePath_string);
    //     }
    //     else if (!strcmp(name_string, "turret.png")){
    //         turret = gf2d_sprite_load_image(spritePath_string);
    //     }
    //     else
    //     {
    //         slog("no matching spritepath");// code to be executed if n doesn't match any cases
    //     }
    // }

    // SJson *pickup_array_config = sj_object_get_value(config, "Pickups");
    // // int i;
    // for (i = 0;i < sj_array_get_count(pickup_array_config);i++){
    //     SJson *config_pickup_data = sj_array_get_nth(pickup_array_config, i);

    //     SJson *pickup_name = sj_object_get_value(config_pickup_data, "Name");
    //     SJson *pickup_spritePath = sj_object_get_value(config_pickup_data, "Sprite");
    //     char *name_string = (char *)sj_get_string_value(pickup_name);
    //     char *spritePath_string = (char *)sj_get_string_value(pickup_spritePath);
    //     // slog("%s", name_string);
    //     if (!strcmp(name_string, "pickup_heal")){
    //         pickup_health = gf2d_sprite_load_image(spritePath_string);
    //     }
    //     else if (!strcmp(name_string, "pickup_boost")){
    //         pickup_boost = gf2d_sprite_load_image(spritePath_string);
    //     }
    //     else if (!strcmp(name_string, "pickup_speed")){
    //         pickup_speed = gf2d_sprite_load_image(spritePath_string);
    //     }
    //     else
    //     {
    //         slog("no matching spritepath");// code to be executed if n doesn't match any cases
    //     }
    // }
}

Entity *projectile_generic(
    Entity *owner_entity,
    TextWord name,
    Sprite *sprite,
    int collider_shape,
    int radius_body,
    int radius_range,
    Vector2D draw_offset,
    float strength,
    float speed,
    int time_to_live,
    Vector2D init_pos,
    void (*think)(struct Entity_S *self),
    void (*touch)(struct Entity_S *self, struct Entity_S *other),
    void (*detect)(struct Entity_S *self, struct Entity_S *other)
    )
    {
        Entity *self;
        self = entity_new();
        if (!self)return NULL;
        //Init ent
        strcpy(self->name, name);
        self->sprite = sprite;
        self->collider_shape = collider_shape;
        if (collider_shape == SHAPE_CIRCLE)self->radius_body = radius_body;
        self->position = init_pos;
        self->drawOffset = draw_offset;   
        self->radius_body = radius_body;  
        self->radius_range = radius_range;   

        //Declaration of projectile
        Projectile *projectile;
        projectile = (Projectile * )malloc(sizeof(Projectile));
        //Initialization of projectile
        projectile->owner_entity = owner_entity;
        vector2d_copy(self->size, owner_entity->size);
        projectile->speed = speed;
        projectile->time_to_live = time_to_live;
        projectile->time_alive = 0;
        projectile->strength = strength;
        self->typeOfEnt = (Projectile *)projectile;
        self->type = ENT_PROJECTILE;
        self->team = owner_entity->team;
        self->think = think;
        self->touch = touch;
        self->detect = detect;
        self->health = 75; //TODO: fix
        // slog("%s: %f.%f %f.%f",self->name, self->position.x, self->position.y,self->size.x, self->size.y);
        // slog("%s team %d", self->name, self->team);
        // slog("%s Going to %f.%f",self->name, self->size.x, self->size.y);
        return self;
    }

Entity *hitscan_generic(
    Entity *owner_entity,
    TextWord name,
    int collider_shape,
    float strength,
    int time_to_live,
    float angleOffset,
    void (*think)(struct Entity_S *self),
    void (*touch)(struct Entity_S *self, struct Entity_S *other),
    int type
    ){
        Entity *self;
        self = entity_new();
        if (!self)return NULL;
        //Init ent
        self->sprite = blank;
        strcpy(self->name, name);
        // self->sprite = sprite;
        self->collider_shape = collider_shape;
        // if (collider_shape == SHAPE_CIRCLE)self->radius = radius;
        self->position = owner_entity->position;
        Vector2D vScale;

        
        self->size.x = cos((vector2d_angle(owner_entity->size) + angleOffset)*M_PI/180);
        self->size.y = sin((vector2d_angle(owner_entity->size) + angleOffset)*M_PI/180);
        vector2d_scale(vScale, self->size, 200);

        vector2d_add(self->size, self->position, vScale);

        //Declaration of hitscan
        Projectile *hitscan;
        hitscan = (Projectile * )malloc(sizeof(Projectile));
        //Initialization of hitscan
        hitscan->owner_entity = owner_entity;
        // vector2d_copy(self->size, owner_entity->size);
        hitscan->time_to_live = time_to_live;
        hitscan->time_alive = 0;
        hitscan->strength = strength;

        hitscan->firstContact = (Entity *)malloc(sizeof(Entity));

        self->typeOfEnt = (Projectile *)hitscan;
        self->type = type;
        self->team = owner_entity->team;
        self->think = think;
        self->touch = touch;
        return self;
    }

void think_move_constVel(Entity *self){
    Projectile *p = (Projectile *)self->typeOfEnt;
    p->time_alive += 1;
    if (p->time_alive > p->time_to_live){
            self->_inuse = 0;  
            return;
    }
    self->position.x += self->size.x * p->speed;
    self->position.y += self->size.y * p->speed;    
    // slog("%s %d",self->name, level_get_active()->frame);
}

void think_stationary(Entity *self){
    Projectile *p = (Projectile *)self->typeOfEnt;
    p->time_alive += 1;
    if (p->time_alive > p->time_to_live){
        Player *owner_player = (Player *)p->owner_entity->typeOfEnt;
        owner_player->deployables -= 1;
        self->_inuse = 0;
        return;
    }
}

// void fireball_think(Entity *self){

// }

// void healingAura_think(Entity *self){
//     Projectile *p = (Projectile *)self->typeOfEnt;
//     p->time_alive += 1;
//     if (p->time_alive > p->time_to_live){
//         self->_inuse = 0;
//         return;
//     }
// }

// void damageAura_think(Entity *self){
//     Projectile *p = (Projectile *)self->typeOfEnt;
//     p->time_alive += 1;
//     if (p->time_alive > p->time_to_live){
//         self->_inuse = 0;
//         return;
//     }
// }

// void hitscan_think(Entity *self){
//     Projectile *p = (Projectile *)self->typeOfEnt;
//     p->time_alive += 1;

//     if (p->time_alive > p->time_to_live){
//         self->_inuse = 0;
//         return;
//     }
// }

void rayscan_think(Entity *self){
    Projectile *p = (Projectile *)self->typeOfEnt;
    p->time_alive += 1;
    slog("hit %s ", p->firstContact->name);
    p->firstContact->health -= 0.1;
    if (p->time_alive > p->time_to_live){
        self->_inuse = 0;
        return;
    }
}

void turret_think(Entity *self){
    Projectile *p = (Projectile *)self->typeOfEnt;
    // p->time_alive += 1;

    if (self->health <= 0){
        Player *owner_player = (Player *)p->owner_entity->typeOfEnt;
        owner_player->deployables -= 1;
        self->_inuse = 0;
        return;
    }
}

void fireball_touch(Entity *self, Entity *other){
    if(!self)return;
    if (!other)return;
    Projectile *p = (Projectile *)self->typeOfEnt;
    Entity *owner_ent = (Entity *)p->owner_entity;

    if (other == owner_ent)return;

    if (other->team != owner_ent->team){
        if (other->type == ENT_PLAYER ){
            // Player *other_player = (Player *)other->typeOfEnt;
            other->health -= p->strength;
            self->_inuse = 0;
            return;
        }
        else if (other->type == ENT_CORE ){
            // Level_core *other_core = (Level_core *)other->typeOfEnt;
            other->health -= p->strength;
            slog("%s health: %f", other->name, other->health);
            self->_inuse = 0;
            return;
        }
        else if (other->type == ENT_PROJECTILE){
            self->_inuse = 0;
            return;
        }
    }
}

void healingAura_touch(Entity *self, Entity *other){
    if (!self)return;
    Projectile *p = (Projectile *)self->typeOfEnt;
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

void damageAura_touch(Entity *self, Entity *other){
    if (!self)return;
    Projectile *p = (Projectile *)self->typeOfEnt;
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

void speedAura_touch(Entity *self, Entity *other){
    if (!self)return;
    // Projectile *p = (Projectile *)self->typeOfEnt;

    if (self->team == other->team){
        if (other->type == ENT_PLAYER){
            Player *other_player = (Player *)other->typeOfEnt;
            other_player->speed = 1.5;
        }
    }
}

void hitscan_touch(Entity *self, Entity *other){
    //TODO: circle detecction
    if (!self)return;
    if (!other)return;
    Projectile *p = (Projectile *)self->typeOfEnt;
    Entity *owner_ent = (Entity *)p->owner_entity;
    // slog("Self team %d other ");

    if (self->team != other->team && other != owner_ent){

        if (other->type == ENT_PLAYER){
            // Player *other_player = (Player *)other->typeOfEnt;
            other->health -= p->strength;
            // slog("Damaged %s %f", other->name, other->health);
        }
        else if (other->type == ENT_CORE){
            // Level_core *other_core = (Level_core *)other->typeOfEnt;
            other->health -= p->strength;
            // slog("Damaged %s %f", other->name, other_player->health);
        }
    }
}

void rayscan_touch(Entity *self, Entity *other){
    if (!self)return;
    if (!other)return;
    // Projectile *p = (Projectile *)self->typeOfEnt;
    // Entity *owner_ent = (Entity *)p->owner_entity;
}

void turret_touch(Entity *self, Entity *other){
    
}

void heal_dart_touch(Entity *self, Entity *other){
    if(!self)return;
    if (!other)return;
    Projectile *p = (Projectile *)self->typeOfEnt;
    Entity *owner_ent = (Entity *)p->owner_entity;

    if (other == owner_ent)return;

    if (other->team != self->team){
        if (other->type == ENT_PLAYER ){
            // Player *other_player = (Player *)other->typeOfEnt;
            other->health -= p->strength;
            self->_inuse = 0;
            return;
        }
        else if (other->type == ENT_CORE ){
            // Level_core *other_core = (Level_core *)other->typeOfEnt;
            other->health -= p->strength;
            self->_inuse = 0;
            return;
        }
        else if (other->type == ENT_PROJECTILE){
            self->_inuse = 0;
            return;
        }
    }
    else if (other->team == self->team){
        if (other->type == ENT_PLAYER ){
            // Player *other_player = (Player *)other->typeOfEnt;
            other->health += p->strength;
            self->_inuse = 0;
            return;
        }
        else if (other->type == ENT_CORE ){
            // Level_core *other_core = (Level_core *)other->typeOfEnt;
            // other->health -= p->strength;
            self->_inuse = 0;
            return;
        }
        else if (other->type == ENT_PROJECTILE){
            self->_inuse = 0;
            return;
        }
    }
    
}

void landmine_touch(Entity *self, Entity *other){
    if (!self)return;
    Projectile *p = (Projectile *)self->typeOfEnt;
    Entity *owner_ent = (Entity *)p->owner_entity;
    if (other == owner_ent)return;
    if (other->team == self->team)return;
    // slog("%s triggered by %s", self->name, other->name);
    // slog("%d vs %d", self->team, other->team);

    if (other->type == ENT_PLAYER && other->team != self->team){
        int i;
        for (i = 0;i < entity_manager_get_active().maxEnts;i++)
        {    

            if (!entity_manager_get_active().entityList[i]._inuse)continue;
            if (&entity_manager_get_active().entityList[i] == self)continue;
            if (entity_manager_get_active().entityList[i].type != ENT_PLAYER)continue;
            if (entity_manager_get_active().entityList[i].team == self->team)continue;

            if (collide_circle(self->position, self->radius_range, entity_manager_get_active().entityList[i].position, entity_manager_get_active().entityList[i].radius_body)){
                entity_manager_get_active().entityList[i].health -= p->strength;
                // slog("%s exploded on %s", self->name, entity_manager_get_active().entityList[i].name);
            }
        }
    }   
    Player *owner_player = (Player *)p->owner_entity->typeOfEnt;
    owner_player->deployables -= 1;
    self->_inuse = 0; 
}
void detect_test(Entity *self, Entity *other){
    slog("%s",other->name);
}
void turret_detect(Entity *self, Entity *other){
    if (!self)return;
    Projectile *p = (Projectile *)self->typeOfEnt;
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
        "Tfire",
        fireball,
        SHAPE_CIRCLE,
        25,
        0,
        vector2d(-25,-25),
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

Entity *level_pickup_new(
    TextWord    name,
    Sprite      *sprite,
    Vector2D    position,
    void        (*touch)(struct Entity_S *self, struct Entity_S *other)
){
    Entity *self;
    self = entity_new();
    if (!self)return NULL;
    strcpy(self->name, name);
    self->sprite = sprite;
    self->position = position;
    self->touch = touch;
    self->type = ENT_PICKUP;
    self->collider_shape = SHAPE_CIRCLE;
    self->radius_body = 30;
    self->drawOffset = vector2d(-30,-30);
    return self;
}


void pickup_health_touch(Entity *self, Entity *other){
    if (!self)return;
    // Player *other_player = (Player *)other->typeOfEnt;

    if (other->type == ENT_PLAYER){
        other->health += 50;
        self->_inuse = 0;
    }
    // // if (other->type == ENT_PLAYER)
    // // slog("%s picked up by %s", self->name, other->name);
}
void pickup_boost_touch(Entity *self, Entity *other){
    if (!self)return;
    Player *other_player = (Player *)other->typeOfEnt;

    if (other->type == ENT_PLAYER){
        other_player->strength += 0.25;
        self->_inuse = 0;
    }
}
void pickup_speed_touch(Entity *self, Entity *other){
    if (!self)return;
    // slog("yes");
    Player *other_player = (Player *)other->typeOfEnt;

    if (other->type == ENT_PLAYER){
        other_player->speed = 1.25;
        // slog("sped %s by %f", other->name, other_player->speed);
        self->_inuse = 0;
    }
}
