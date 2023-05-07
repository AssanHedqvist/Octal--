#ifndef RENDEROBJECT_H_INCLUDED
#define RENDEROBJECT_H_INCLUDED

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

typedef struct
{
    unsigned int order; //  rendering order of the renderObject starting at 0
    SDL_Texture* texture;
    SDL_Rect imageExtents;
    SDL_Rect screenExtents;
    unsigned char flip;
} RenderObject;

int spriteHandler(RenderObject object, int spriteToGet);

void renderObject(SDL_Renderer *renderer, RenderObject* objectToRender);

void render(SDL_Renderer *renderer, RenderObject objects[], int length);


#endif