#ifndef _MOUSE_H_
#define _MOUSE_H_

#include "gf2d_sprite.h"
#include "level.h"


typedef struct Mouse_S
{
    Sprite      *sprite;            /**<A pointer to the sprite*/
    float       frame;              /**<Current frame of sripte*/
    float       maxFrames;          /**<Maximum number of frames in a sprite*/
    Vector2D    position;           /**<2D position of entity*/
}Mouse;

Mouse *mouse_new();

void mouse_free(Mouse *self);

#endif