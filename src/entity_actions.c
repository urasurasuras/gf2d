#include <stdlib.h>
#include "simple_logger.h"
#include "entity_actions.h"

void move_up(Entity *self, float speed){
    self->position.y -= speed;
}
void move_down(Entity *self, float speed){
    self->position.y += speed;
}
void move_left(Entity *self, float speed){
    self->position.x -= speed;
}
void move_right(Entity *self, float speed){
    self->position.x += speed;
}
void respawn(Entity *self){
    entity_free(self);
    self = newTestEnt();
    slog("Respawned ent");
}
