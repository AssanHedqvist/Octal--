#include "../include/keyboard.h"

void initKeyboardStates(KeyboardStates* states) 
{
    for (int i = 0; i < 8; i++)
    {
        states->keyState[i] = 0;
    }
}

void setKeyboardKey(KeyboardStates* states, SDL_Scancode keyCode, int eventType) 
{
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

char getKeyboardKey(KeyboardStates* states, SDL_Scancode keyCode) 
{
    return (states->keyState[keyCode >> 5u] & (1u << (keyCode & 31u))) > 0;
}