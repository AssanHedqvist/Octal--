#ifndef MENU_H_INCLUDED
#define MENU_H_INCLUDED

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "../include/text.h"


SDL_Rect* renderMenu(SDL_Renderer* renderer, Text MenuText);

#endif