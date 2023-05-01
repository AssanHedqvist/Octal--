#ifndef MENU_H_INCLUDED
#define MENU_H_INCLUDED

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "../include/text.h"


void renderMenu(SDL_Renderer* renderer, Text menuText, SDL_Rect* buttons);

#endif