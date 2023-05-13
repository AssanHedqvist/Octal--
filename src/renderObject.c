#include "../include/renderObject.h"

void initRenderObjects(RenderObject objects[], SDL_Renderer *renderer) 
{
    //   Render order: start at 0 continue up.
    objects[0].order = 0;
    objects[0].texture = IMG_LoadTexture(renderer, "resources/background.png");
    objects[0].imageExtents = (SDL_Rect){0, 0, 6000, 3000};
    objects[0].screenExtents = (SDL_Rect){0, 0, 800, 600};
    objects[0].flip = 0;

    objects[1].order = 1;
    objects[1].texture = IMG_LoadTexture(renderer, "resources/platform.png");
    objects[1].imageExtents = (SDL_Rect){0, 0, 1000, 75};
    objects[1].screenExtents = (SDL_Rect){100, 300, 600, 53};
    objects[1].flip = 0;

    objects[2].order = 2;
    objects[2].texture = IMG_LoadTexture(renderer, "resources/gridnetForSpritesAlpha.png");
    objects[2].imageExtents = (SDL_Rect){0, 0, 128, 256};
    objects[2].screenExtents = (SDL_Rect){400, 300, 32, 64};
    objects[2].flip = 0;

    objects[3].order = 2;
    objects[3].texture = IMG_LoadTexture(renderer, "resources/gridnetForSpritesAlpha.png");
    objects[3].imageExtents = (SDL_Rect){0, 0, 128, 256};
    objects[3].screenExtents = (SDL_Rect){300, 300, 32, 64};
    objects[3].flip = 0;

    objects[4].order = 2;
    objects[4].texture = IMG_LoadTexture(renderer, "resources/gridnetForSpritesAlpha.png");
    objects[4].imageExtents = (SDL_Rect){0, 0, 128, 256};
    objects[4].screenExtents = (SDL_Rect){300, 300, 32, 64};
    objects[4].flip = 0;

    objects[5].order = 2;
    objects[5].texture = IMG_LoadTexture(renderer, "resources/gridnetForSpritesAlpha.png");
    objects[5].imageExtents = (SDL_Rect){0, 0, 128, 256};
    objects[5].screenExtents = (SDL_Rect){300, 300, 32, 64};
    objects[5].flip = 0;
}

void renderObject(SDL_Renderer *renderer, RenderObject* objectToRender) 
{
    SDL_RenderCopyEx(renderer,objectToRender->texture,&objectToRender->imageExtents,&objectToRender->screenExtents,0.0,NULL,0);
}

void render(SDL_Renderer *renderer, RenderObject objects[], int length) 
{
    int currentOrder = 0;
    int doneRendering = 0;
    
    while(!doneRendering) 
    {
        doneRendering = 1;
        for (int i = 0; i < length; i++)
        {
            if(objects[i].order == currentOrder) 
            {
                SDL_RenderCopyEx(renderer,objects[i].texture,&objects[i].imageExtents,&objects[i].screenExtents,0.0,NULL,objects[i].flip);
            }
            if(currentOrder <= objects[i].order) 
            {
                doneRendering = 0;
            }     
        }
        currentOrder++;
    }
}