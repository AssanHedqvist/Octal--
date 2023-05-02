#ifndef MOUSE_H_INCLUDED
#define MOUSE_H_INCLUDED

//  don't know what to put here yet --Damien

#include <SDL2/SDL.h>

typedef struct {
    int x;
    int y;
    unsigned char state;
} MouseState;

void updateMousePos(MouseState* mouseState);

void handleMouseInputs(MouseState* states, int mouseButton, int eventType);

char isMouseButtonPressed(MouseState* mouseState, int mouseButton);

#endif