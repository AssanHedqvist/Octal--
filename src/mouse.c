#include "../include/mouse.h"

void updateMousePos(MouseState* mouseState) 
{
    SDL_GetMouseState(&mouseState->x, &mouseState->y);
}

void handleMouseInputs(MouseState* mouseState, int mouseButton, int eventType) 
{
    if(eventType == SDL_MOUSEBUTTONDOWN) {
        mouseState->state |= (1 << mouseButton);
    }
    else {
        mouseState->state &= (~(1 << mouseButton));
    }
}

char isMouseButtonPressed(MouseState* mouseState, int mouseButton) {
    return (mouseState->state & (1u << mouseButton )) > 0;
}