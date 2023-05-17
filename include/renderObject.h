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
    unsigned char flags;
} RenderObject;

typedef enum {
    FLIP = 0b00000001,
    RENDER_ACTIVE = 0b00001000
} renderFlags;

static inline unsigned char flagRenderGet(unsigned char objFlags, unsigned char renderFlagToCheck) 
{
    return ((objFlags & renderFlagToCheck) > 0);
}

void initRenderObjects(RenderObject objects[], SDL_Renderer *renderer);

void renderObject(SDL_Renderer *renderer, RenderObject* objectToRender);

void render(SDL_Renderer *renderer, RenderObject objects[], int length);


#endif