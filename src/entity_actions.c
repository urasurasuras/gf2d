#include <stdlib.h>
#include "simple_logger.h"
#include "entity_actions.h"

void move_up(Entity *self){
    self->position.y -= self->speed;
}
void move_down(Entity *self){
    self->position.y += self->speed;
}
void move_left(Entity *self){
    self->position.x -= self->speed;
}
void move_right(Entity *self){
    self->position.x += self->speed;
}
void respawn(Entity *self){
    entity_free(self);
    slog("Respawned ent");
}
