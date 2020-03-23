#include "enemy.h"

void enemy_think (Entity *self){
    //move
    int random_x = 1;
    int random_y = 1;
    int random_dir;
    // srand((unsigned) /*level_get_active()->frame * */time(NULL));

    random_dir = rand() %2;
    if (random_dir){
        random_x = rand() % 16;
    }
    else
    {
        random_x = rand() % 16;
        random_x = -random_x;
    }
    random_dir = rand() %2;
    if (random_dir){
        random_y = rand() % 9;
    }
    else
    {
        random_y = rand() % 9;
        random_y = -random_y;
    }

    self->position.x += random_x;
    self->position.y += random_y;

    if (level_bounds_test_circle(level_get_active()->bounds_level, self->position, self->radius_body))
    {
        //TODO: Do something is ent hits bounds
        self->position.x -= random_x;
        self->position.y -= random_y;
    }
}

void enemy_touch (Entity *self, Entity *other){
    //damage and push back
    slog("enemy touchgin");
    if (other->type == ENT_PLAYER){
        // Player *p = (Player *) other->typeOfEnt;
        other->health -= 0.1;
    }
}

void enemy_detect(Entity *self, Entity *other){
    //throw fireballs
    slog("enemy detecting");
    static int last_enemy_shoot;
    if (self->team != other->team && last_enemy_shoot + 60 < level_get_active()->frame
        && other->type == ENT_PLAYER){

        Vector2D v;
        vector2d_sub(v, other->position, self->position); 

        self->size.x = cos(vector2d_angle(v) * M_PI/180);
        self->size.y = sin(vector2d_angle(v) * M_PI/180);
        // vector2d_copy(self->size, other->position);
        slog("Turret pos %f.%f %s pos %f.%f", self->size.x, self->size.y, other->name, other->position.x, other->position.y);

        projectile_generic(
        self,
        "EnemyFire",
        fireball,
        SHAPE_CIRCLE,
        25,
        NULL,
        vector2d(-25,-25),
        25,
        3,
        self->position,
        fireball_think,
        fireball_touch,
        NULL
        ); 

        last_enemy_shoot = level_get_active()->frame;
    }
}