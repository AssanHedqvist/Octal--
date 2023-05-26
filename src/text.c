#include "../include/text.h"

void renderPlayerHealth(Player players[4], SDL_Renderer* renderer, TTF_Font* font, SDL_Texture* livesIcon[4])
{
    char stringBuffer[12];

    int alignment = 0;
    int x = 0;
    int amountOfPlayers = 0;

    for (int i = 0; i < 4; i++)
    {
        if(flagRenderGet(players[i].render->flags, RENDER_ACTIVE)) 
        {
            amountOfPlayers++;
        }
    }

    switch (amountOfPlayers)
    {
    case 1:
        x = 350;
        //  don't care about alignment here
        alignment = 0;
        break;
    case 2:
        x = 250;
        alignment = 200;
        break;
    case 3:
        x = 150;
        alignment = 200;
        break;
    case 4:
        x = 100;
        alignment = 166;
        break;
    default:
        break;
    }

    SDL_Rect healthRect = {x, 550, 100, 50};
    SDL_Rect lifeRect = {x, 525, 25, 25}; 

    for(int i = 0; i < 4; i++)
    {
        if(flagRenderGet(players[i].render->flags, RENDER_ACTIVE)) 
        {
            snprintf(stringBuffer, sizeof(stringBuffer), "P%d% d%%", i+1, players[i].health);
        
            SDL_Color color = {255, 255-players[i].health, 255-2*players[i].health};
            SDL_Surface* surface = TTF_RenderText_Solid(font, stringBuffer, color);
            SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

            for (int j = 0; j < players[i].lives; j++)
            {
                SDL_RenderCopyEx(renderer, livesIcon[i], NULL, &lifeRect, 0.0, NULL, 0);
                lifeRect.x += 25;
            }
            
            SDL_RenderCopyEx(renderer, texture, NULL, &healthRect, 0.0, NULL, 0);

            SDL_FreeSurface(surface);

            SDL_DestroyTexture(texture);

            healthRect.x += alignment;
            lifeRect.x += (alignment - 100);
        }
    }
}
