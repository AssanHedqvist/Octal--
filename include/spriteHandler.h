#ifndef SPRITEHANDLER_H_INCLUDED
#define SPRITEHANDLER_H_INCLUDED

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "../include/renderObject.h"


int spriteHandler(RenderObject objects, int spriteToGet)
{
    return objects.screenExtents.w * spriteToGet;
    //return spriteToGet;
}


#endif