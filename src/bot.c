#include "bot.h"

Entity *lavaGuy_new(){
    Entity *enemy = entity_new();
    strcpy(enemy->name, "Enemy");
    enemy->sprite = gf2d_sprite_load_image("images/enemies/enemy.png");
    enemy->position = vector2d(LEVEL_WIDTH/2, LEVEL_HEIGHT/2);
    enemy->radius_body = 64; //TODO: radius per sprite
    // enemy->radius_range = 256;
    enemy->collider_shape = SHAPE_CIRCLE;
    enemy->drawOffset = vector2d(-64,-64);
    enemy->think = enemy_think;
    enemy->touch = enemy_touch;
    enemy->bound_hit = player_bound_hit;
    enemy->detect = enemy_detect;
    enemy->radius_range = 500;
    enemy->health = 500;
    // enemy->detect = enemy_detect;
    enemy->type = ENT_NEUTRAL_MONSTER;
    Bot *bot;
    bot = (Bot * )malloc(sizeof(Bot));
    bot->cldn_action = 300;
    bot->cldn_movement = 30;
    bot->cldn_randomized = 30;
    enemy->typeOfEnt = bot;
    enemy->collidable = 1;

    return enemy;
}

Entity *grassGuy_new(){
    Entity *enemy = entity_new();
    strcpy(enemy->name, "NotEnemy");
    enemy->sprite = gf2d_sprite_load_image("images/enemies/not_enemy.png");
    enemy->position = vector2d(LEVEL_WIDTH/2, LEVEL_HEIGHT/2);
    enemy->radius_body = 64; //TODO: radius per sprite
    // enemy->radius_range = 256;
    enemy->collider_shape = SHAPE_CIRCLE;
    enemy->drawOffset = vector2d(-64,-64);
    enemy->think = not_enemy_think;
    enemy->touch = not_enemy_touch;
    enemy->bound_hit = player_bound_hit;
    enemy->health = 500;
    // enemy->detect = enemy_detect;
    enemy->type = ENT_NEUTRAL_MONSTER;
    Bot *bot;
    bot = (Bot * )malloc(sizeof(Bot));
    bot->cldn_action = 300;
    bot->cldn_movement = 180;
    bot->cldn_randomized = 180;
    bot->last_action =0;
    bot->last_movement =0;
    bot->last_randomized=0;
    enemy->typeOfEnt = bot;
    enemy->collidable = 1;

    return enemy;
}

void enemy_think (Entity *self){
    Bot *b = (Bot *)self->typeOfEnt;
    //move
    // int ne_random_x = 1;
    // int ne_random_y = 1;
    int ne_random_dir;
    // srand((unsigned) /*level_get_active()->frame * */time(NULL));
    if (b->last_randomized + b->cldn_randomized < level_get_active()->frame ){
        ne_random_dir = rand() %2;
        slog("direction %d", ne_random_dir);
        if (ne_random_dir){
            self->size.x = rand() % 16;
        }
        else
        {
            self->size.x = rand() % 8;
            self->size.x = -self->size.x;
        }
        ne_random_dir = rand() %2;
        if (ne_random_dir){
            self->size.y = rand() % 9;
        }
        else
        {
            self->size.y = rand() % 4;
            self->size.y = -self->size.y;
        }
        slog("vector %d.%d", self->size.x, self->size.y);

        b->last_randomized = level_get_active()->frame;
    }
    
    //if (level_bounds_test_circle(level_get_active()->bounds_level, self->position, self->radius_body))
    //{
    //    //TODO: Do something is ent hits bounds
    //    self->position.x -= self->size.x;
    //    self->position.y -= self->size.y;
    //}

   
    if (self->health <= 0) {

        for (int i = 0; i < entity_manager_get_active()->maxEnts; i++) {
            if (entity_manager_get_active()->entityList[i].team == self->lastHit) {
                //give buff to that team
                entity_manager_get_active()->entityList[i].health += 100;
            }
        }
        self->_inuse = 0;//then die
    }

    //if (freeze)return;
    self->position.x += self->size.x;
    self->position.y += self->size.y;
}

void enemy_touch (Entity *self, Entity *other){
    //damage and push back
    // slog("enemy touchgin");
    if (other->type == ENT_PLAYER){
        // Player *p = (Player *) other->typeOfEnt;
        other->health -= 0.1;
    }
}

void enemy_detect(Entity *self, Entity *other){
    Bot *b = (Bot *)self->typeOfEnt;
    //throw fireballs
    // slog("enemy detecting");
    if (self->team != other->team && b->last_action + b->cldn_action < level_get_active()->frame
        && other->type == ENT_PLAYER){

        Vector2D v;
        vector2d_sub(v, other->position, self->position); 

        self->size.x = cos(vector2d_angle(v) * M_PI/180);
        self->size.y = sin(vector2d_angle(v) * M_PI/180);
        // vector2d_copy(self->size, other->position);
        // slog("Turret pos %f.%f %s pos %f.%f", self->size.x, self->size.y, other->name, other->position.x, other->position.y);

        if (b->last_action + b->cldn_action < level_get_active()->frame) {
            Entity * fireball = fireball_projectile(self);
            /*fireball->rotation.x = self->size.x;
            fireball->rotation.y = self->size.y;*/
            fireball->rotation.z = vector2d_angle(self->size) - 45;
            b->last_action = level_get_active()->frame;
        }

        b->last_action = level_get_active()->frame;
    }
}

void not_enemy_think (Entity *self){
    Bot *b = (Bot *)self->typeOfEnt;
    // slog("%d %d %d",b->last_action,b->last_movement,b->last_randomized);
    //move
    // int ne_random_x = 1;
    // int ne_random_y = 1;
    int ne_random_dir;
    // srand((unsigned) level_get_active()->frame * time(NULL));
    if (b->last_randomized + b->cldn_randomized < level_get_active()->frame ){
        ne_random_dir = rand() %2;
        // slog("direction %d", ne_random_dir);
        if (ne_random_dir){
            self->size.x = rand() % 16;
        }
        else
        {
            self->size.x = rand() % 8;
            self->size.x = -self->size.x;
        }
        ne_random_dir = rand() %2;
        if (ne_random_dir){
            self->size.y = rand() % 9;
        }
        else
        {
            self->size.y = rand() % 4;
            self->size.y = -self->size.y;
        }
        // slog("vector %d.%d", self->size.x, self->size.y);

        b->last_randomized = level_get_active()->frame;
    }
    

    //if (level_bounds_test_circle(level_get_active()->bounds_level, self->position, self->radius_body))
    //{
    //    //TODO: Do something is ent hits bounds
    //    self->position.x -= self->size.x;
    //    self->position.y -= self->size.y;
    //}

    if (b->last_action + b->cldn_action < level_get_active()->frame){
        // Vector2D v;
        // vector2d_sub(v, other->position, self->position); 

        // self->size.x = cos(vector2d_angle(v) * M_PI/180);
        // self->size.y = sin(vector2d_angle(v) * M_PI/180);

        projectile_generic(
        self,
        "nEnemyHeal",
        pickup_health,
            NULL,
        SHAPE_CIRCLE,
        30,
        0,
        25,
        1,
        LEVEL_WIDTH/3,
        self->position,
        think_move_constVel,
        pickup_health_touch,
        NULL
        ); 

        b->last_action = level_get_active()->frame;
    }

    if (self->health <= 0) {
        
        for (int i = 0; i < entity_manager_get_active()->maxEnts; i++) {
            if (entity_manager_get_active()->entityList[i].team == self->lastHit) {
                //give buff to that team
                Player* p;
                Companion* c;
                switch (entity_manager_get_active()->entityList[i].type)
                {
                case ENT_PLAYER:
                    p = (Player*)entity_manager_get_active()->entityList[i].typeOfEnt;
                    p->strength = 2;
                    break;
                case ENT_COMPANION:
                    c = (Companion *)entity_manager_get_active()->entityList[i].typeOfEnt;
                    c->strength = 3;
                    break;
                default:
                    break;
                }
            }
        }
        self->_inuse = 0;//then die
    }
    //if (freeze)return;
    self->position.x += self->size.x;
    self->position.y += self->size.y;
}

void not_enemy_touch (Entity *self, Entity *other){
    //slog("%s touched by %s", self->name, other->name);
    slog("%.0f", self->health);
    //push back and heal
}

void not_enemy_detect(Entity *self, Entity *other){
    // Bot *b = (Bot *)self->typeOfEnt;
    //throw fireballs
    // slog("enemy detecting");
    // if (self->team != other->team && b->last_action + b->cldn_action < level_get_active()->frame
    //     && other->type == ENT_PLAYER){


    // }
}
