#ifndef RENDEROBJECT_H_INCLUDED
#define RENDEROBJECT_H_INCLUDED

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

typedef struct RenderObject
{
    unsigned int order; //  rendering order of the renderObject starting at 0
    SDL_Texture* texture;
    SDL_Rect imageExtents;
    SDL_Rect screenExtents;
    SDL_RendererFlip flip;
} RenderObject;

void renderObject(SDL_Renderer *renderer, RenderObject* objectToRender);

void renderObjects(SDL_Renderer *renderer, RenderObject objects[], int length);



#endif