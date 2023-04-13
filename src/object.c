#include "../include/object.h"

void renderObject(SDL_Renderer *renderer, Object* objectToRender) 
{
    SDL_RenderCopyEx(renderer,objectToRender->texture,&objectToRender->imageExtents,&objectToRender->screenExtents,0.0,NULL,1);
}