#ifndef KEYBOARD_H_INCLUDED
#define KEYBOARD_H_INCLUDED

#include <SDL2/SDL.h>


typedef struct
{
    unsigned int keyState[8];
} KeyboardStates;

void handleKeyboardInputs(KeyboardStates* states, SDL_Scancode keyCode, int eventType);

char isKeyDown(KeyboardStates* states, SDL_Scancode keyCode);

#endif