#ifndef OBJECT_H_INCLUDED
#define OBJECT_H_INCLUDED

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

typedef struct Object
{
    SDL_Texture* texture;
    SDL_Rect imageExtents;
    SDL_Rect screenExtents;
    SDL_RendererFlip flip; 
    unsigned int type; //  for example static objects (immovable objects) and dynamic objects (movable objects)
    unsigned int order; //  order is the order which things are drawn on the screen where 0 is the first element drawn (for example background)
} Object;

void renderObject(SDL_Renderer *renderer, Object* objectToRender);

void renderObjects(SDL_Renderer *renderer, Object objects[], int length);

#endif