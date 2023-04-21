#include "../include/spriteHandler.h"

int spriteHandler(RenderObject object, int spriteToGet)
{
    return object.screenExtents.w * spriteToGet;
}
