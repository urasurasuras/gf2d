#include <stdlib.h>
#include "simple_logger.h"
#include "menu.h"
#include <SDL.h>
#include "gf2d_draw.h"
#include "collision.h"
#include "level.h"
#include "mouse.h"

static Mouse *mouse = {0};

Mouse *mouse_new(){
    mouse = (Mouse *)malloc(sizeof(Mouse));
    // memset(mouse,0,sizeof(Mouse));
    mouse->position = vector2d(100,100);
    return mouse;
}

void mouse_free(Mouse *self){
    if (!self)return;
    gf2d_sprite_free(self->sprite);
    memset(self,0,sizeof(Mouse));
}

