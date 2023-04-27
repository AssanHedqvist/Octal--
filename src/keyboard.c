#include "../include/keyboard.h"

void handleKeyboardInputs(KeyboardStates* states, SDL_Scancode keyCode, int eventType) 
{
    //  this works because SDL_KEYUP - SDL_KEYUP = 0 and SDL_KEYUP - SDL_KEYDOWN = 1 
    if(keyCode <= 230) //  if false ignore that input
    { 
        if(eventType == SDL_KEYDOWN) {
            states->keyState[keyCode >> 5u] |= (1u << (keyCode & 31u));
        }
        else {
            states->keyState[keyCode >> 5u] &= (~(1u << (keyCode & 31u)));
        }
    }
}

char isKeyDown(KeyboardStates* states, SDL_Scancode keyCode) 
{
    return (states->keyState[keyCode >> 5u] & (1u << (keyCode & 31u))) > 0;
}