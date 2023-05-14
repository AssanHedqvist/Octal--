#ifndef ATTACKS_H_INCLUDED
#define ATTACKS_H_INCLUDED

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "../include/keyboard.h"
#include "../include/player.h"
#include "../include/sounds.h"

void lightPunchServer(Player players[4],unsigned char playerFlip[4], KeyboardStates keyboardInputs[4]);

void lightPunchClient(Player players[4], KeyboardStates* keyboardInputs, unsigned char thisComputersPlayerIndex, SoundEffect soundEffect);

void oldLightPunch(Player players[], int amountOfPlayers, KeyboardStates *keyboardInputs, SoundEffect soundEffect);

#endif