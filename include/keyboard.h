#ifndef KEYBOARD_H_INCLUDED
#define KEYBOARD_H_INCLUDED

#include <SDL2/SDL.h>

typedef struct KeyboardStates
{
    unsigned char keyState[123];
} KeyboardStates;

void handleKeyboardInputs(KeyboardStates* states, SDL_Keycode keycode, int eventType);

#endif