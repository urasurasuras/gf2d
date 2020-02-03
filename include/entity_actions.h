#ifndef _ACTIONS_H_
#define _ACTIONS_H_

#include "entity.h"

void move_up(Entity *self, float speed);
void move_down(Entity *self, float speed);
void move_left(Entity *self, float speed);
void move_right(Entity *self, float speed);

void respawn(Entity *self);

#endif
