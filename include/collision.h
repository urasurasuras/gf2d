#ifndef __COLLISIONS_H__
#define __COLLISIONS_H__

#include "gfc_vector.h"

int collide_circle(Vector2D p1, float r1, Vector2D p2, float r2);
int collide_rect(SDL_Rect r1,SDL_Rect r2);

#endif