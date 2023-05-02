#ifndef MENU_H_INCLUDED
#define MENU_H_INCLUDED

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "../include/text.h"

typedef struct buttons
{
    SDL_Texture* texture;
    SDL_Surface* surface;
    SDL_Rect rect;
    Text text;

}MenuButton;

void renderMenu(SDL_Renderer* renderer, SDL_Texture* backgroundTexture, SDL_Rect backgroundRect, MenuButton* buttons);
void createButtons(SDL_Renderer* renderer, MenuButton* buttons);

#endif