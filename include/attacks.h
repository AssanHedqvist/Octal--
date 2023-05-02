#ifndef ATTACKS_H_INCLUDED
#define ATTACKS_H_INCLUDED

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "../include/keyboard.h"
#include "../include/player.h"


void lightPunch(Player players[], int amountOfPlayers, KeyboardStates*  keyboardInputs);

#endif