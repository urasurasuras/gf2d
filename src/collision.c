#include "gfc_vector.h"
#include "mouse.h"

int collide_circle(Vector2D p1, float r1, Vector2D p2, float r2){
    if (vector2d_magnitude_squared(vector2d( p2.x - p1.x , p2.y - p1.y)) < ((r2+r1)*(r2+r1))){
        return 1;
    }
    return 0;
}

int collide_rect(SDL_Rect r1,SDL_Rect r2){
    if (((r1.x + r1.w) < r2.x) ||
       ((r2.x + r2.w) < r1.x) ||
       ((r1.y + r1.h) < r2.y) ||
       ((r2.y + r2.h) < r1.y))
    {
        return 0;
    }
    return 1;
}

int collide_menu(SDL_Rect r1, Vector2D m){
    if (((r1.x + r1.w) <     m.x) ||
       ((r1.y + r1.h) <     m.y))
    {
        return 0;
    }
    return 1;
}