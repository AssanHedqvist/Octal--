#ifndef ATTACKS_H_INCLUDED
#define ATTACKS_H_INCLUDED

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "../include/keyboard.h"
#include "../include/player.h"

void lightPunch(Player players[], int amountOfPlayers, KeyboardStates *keyboardInputs, SoundEffect soundEffect);

void lightPunchServer(Player players[4],unsigned char playerFlip[4], KeyboardStates keyboardInputs[4]);

#endif