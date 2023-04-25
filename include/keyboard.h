#ifndef KEYBOARD_H_INCLUDED
#define KEYBOARD_H_INCLUDED

#include <SDL2/SDL.h>

typedef struct 
{
    unsigned char keyState[123];
} KeyboardStates;

typedef struct
{
    unsigned int keyState[8];
} KeyboardStatesAlt;

void handleKeyboardInputs(KeyboardStates* states, SDL_Keycode keyCode, int eventType);

void handleKeyboardInputsAlt(KeyboardStatesAlt* states, SDL_Scancode keyCode, int eventType);

char isKeyDown(KeyboardStatesAlt* states, SDL_Scancode keyCode);

#endif