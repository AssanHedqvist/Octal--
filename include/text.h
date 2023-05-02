#ifndef TEXT_H_INCLUDED
#define TEXT_H_INCLUDED

#include "../include/player.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

typedef struct text
{
    TTF_Font *font;
    char* string;
    SDL_Color color;
}Text;

void renderPlayerHealth(Player players[], int amountOfPlayers, SDL_Renderer* renderer, TTF_Font* font, int x, int y);

#endif

