#include "../include/text.h"

void renderPlayerHealth(Player players[], int amountOfPlayers, SDL_Renderer* renderer, TTF_Font* font, int x, int y)
{
    char stringBuffer[12];
    for(int i = 0; i < amountOfPlayers; i++)
    {
        snprintf(stringBuffer, sizeof(stringBuffer), "P%d% d%%", i+1, players[i].health);
        
        SDL_Color color = {255, 255-players[i].health, 255- 2*players[i].health};
        SDL_Surface* surface = TTF_RenderText_Solid(font, stringBuffer, color);
        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_Rect healthRect = {x  + i * 150, y, 100, 50};
        SDL_RenderCopyEx(renderer, texture, NULL, &healthRect, 0.0, NULL, 0);
        SDL_FreeSurface(surface);
        SDL_DestroyTexture(texture);
    }
}
