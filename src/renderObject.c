#include "../include/renderObject.h"

int spriteHandler(RenderObject object, int spriteToGet)
{
    return object.screenExtents.w * spriteToGet;
}

void renderObject(SDL_Renderer *renderer, RenderObject* objectToRender) 
{
    SDL_RenderCopyEx(renderer,objectToRender->texture,&objectToRender->imageExtents,&objectToRender->screenExtents,0.0,NULL,0);
}

void render(SDL_Renderer *renderer, RenderObject objects[], int length) 
{
    SDL_RenderClear(renderer);

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