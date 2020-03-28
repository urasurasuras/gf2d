#include "bot.h"

Entity *lavaGuy_new(){
    Entity *enemy = entity_new();
    strcpy(enemy->name, "Enemy");
    enemy->sprite = gf2d_sprite_load_image("images/enemies/enemy.png");
    enemy->position = vector2d(LEVEL_WIDTH/2, LEVEL_HEIGHT/2);
    enemy->radius_body = 64; //TODO: radius per sprite
    enemy->radius_range = 256;
    enemy->collider_shape = SHAPE_CIRCLE;
    enemy->drawOffset = vector2d(-64,-64);
    enemy->think = not_enemy_think;
    enemy->touch = enemy_touch;
    enemy->detect = enemy_detect;

    Bot *bot;
    bot = (Bot * )malloc(sizeof(Bot));
    bot->cldn_action = 300;
    bot->cldn_movement = 30;
    bot->cldn_randomized = 30;
    enemy->typeOfEnt = bot;
    return enemy;
}

Entity *grassGuy_new(){
    Entity *enemy = entity_new();
    strcpy(enemy->name, "NotEnemy");
    enemy->sprite = gf2d_sprite_load_image("images/enemies/not_enemy.png");
    enemy->position = vector2d(LEVEL_WIDTH/2, LEVEL_HEIGHT/2);
    enemy->radius_body = 64; //TODO: radius per sprite
    enemy->radius_range = 256;
    enemy->collider_shape = SHAPE_CIRCLE;
    enemy->drawOffset = vector2d(-64,-64);
    enemy->think = not_enemy_think;
    enemy->touch = not_enemy_touch;
    enemy->detect = enemy_detect;

    Bot *bot;
    bot = (Bot * )malloc(sizeof(Bot));
    bot->cldn_action = 3000;
    bot->cldn_movement = 180;
    bot->cldn_randomized = 180;
    enemy->typeOfEnt = bot;
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
    self->position.x += self->size.x;
    self->position.y += self->size.y;

    if (level_bounds_test_circle(level_get_active()->bounds_level, self->position, self->radius_body))
    {
        //TODO: Do something is ent hits bounds
        self->position.x -= self->size.x;
        self->position.y -= self->size.y;
    }
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

        projectile_generic(
        self,
        "nEnemyHeal",
        pickup_health,
        SHAPE_CIRCLE,
        25,
        0,
        vector2d(-25,-25),
        25,
        3,
        self->position,
        fireball_think,
        pickup_health_touch,
        NULL
        ); 

        b->last_action = level_get_active()->frame;
    }
}

void not_enemy_think (Entity *self){
    Bot *b = (Bot *)self->typeOfEnt;
    //move
    // int ne_random_x = 1;
    // int ne_random_y = 1;
    int ne_random_dir;
    // srand((unsigned) level_get_active()->frame * time(NULL));
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
    self->position.x += self->size.x;
    self->position.y += self->size.y;

    if (level_bounds_test_circle(level_get_active()->bounds_level, self->position, self->radius_body))
    {
        //TODO: Do something is ent hits bounds
        self->position.x -= self->size.x;
        self->position.y -= self->size.y;
    }
}

void not_enemy_touch (Entity *self, Entity *other){
    //push back and heal
}

void not_enemy_detect(Entity *self, Entity *other){
    //throw healing orb
}
