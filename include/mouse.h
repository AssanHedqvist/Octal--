#ifndef MOUSE_H_INCLUDED
#define MOUSE_H_INCLUDED

#include <SDL2/SDL.h>

typedef struct 
{
    int x;
    int y;
    unsigned char state;
} MouseState;

int getMouseX(MouseState* mouseState);

int getMouseY(MouseState* mouseState);

void updateMousePos(MouseState* mouseState);

void setMouseKey(MouseState* mouseState, int mouseButton, int eventType);

char getMouseKey(MouseState* mouseState, int mouseButton);

#endif