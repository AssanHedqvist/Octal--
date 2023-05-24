#include "../include/mouse.h"

void initMouse(MouseState* mouseState) 
{

    mouseState->x = 0;
    mouseState->y = 0;
    mouseState->state = 0;
}

int getMouseX(MouseState* mouseState) 
{
    return mouseState->x;
}

int getMouseY(MouseState* mouseState) 
{
    return mouseState->y;
}

void updateMousePos(MouseState* mouseState) 
{
    SDL_GetMouseState(&mouseState->x, &mouseState->y);
}

void setMouseKey(MouseState* mouseState, int mouseButton, int eventType) 
{
    if(eventType == SDL_MOUSEBUTTONDOWN) {
        mouseState->state |= (1 << mouseButton);
    }
    else {
        mouseState->state &= (~(1 << mouseButton));
    }
}

char getMouseKey(MouseState* mouseState, int mouseButton) {
    return (mouseState->state & (1u << mouseButton )) > 0;
}