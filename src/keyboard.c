#include "../include/keyboard.h"

void handleKeyboardInputs(KeyboardStates* states, SDL_Keycode keyCode, int eventType) 
{
    //  this works because SDL_KEYUP - SDL_KEYUP = 0 and SDL_KEYUP - SDL_KEYDOWN = 1 
    states->keyState[keyCode] = SDL_KEYUP - eventType;
}