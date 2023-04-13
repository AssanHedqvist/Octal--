#ifndef OBJECT_H_INCLUDED
#define OBJECT_H_INCLUDED

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

typedef struct Object
{
    SDL_Texture* texture;
    SDL_Rect imageExtents;
    SDL_Rect screenExtents;
    int type; //  for example static objects (immovable objects) and dynamic objects (movable objects)
} Object;

void renderObject(SDL_Renderer *renderer, Object* objectToRender);

#endif