#ifndef TEXT_H_INCLUDED
#define TEXT_H_INCLUDED

#include "../include/player.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

typedef struct text
{
    char* string;
    SDL_Color color;
}Text;

void renderPlayerHealth(Player players[4], SDL_Renderer* renderer, TTF_Font* font, SDL_Texture* livesIcon[4]);

#endif

