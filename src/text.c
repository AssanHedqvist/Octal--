#include "../include/text.h"

#define Y 550

void renderPlayerHealth(Player players[4], SDL_Renderer* renderer, TTF_Font* font, SDL_Texture* livesIcon[4])
{
    char stringBuffer[12];

    float alignment = 0.f;
    float x = 0;
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
        x = 350.0f;
        //  don't care about alignment here
        alignment = 0.0f;
        break;
    case 2:
        x = 250.0f;
        alignment = 200.0f;
        break;
    case 3:
        x = 150.0f;
        alignment = 200.0f;
        break;
    case 4:
        x = 100.0f;
        alignment = 166.666656494140625f;
        break;
    default:
        break;
    }

    SDL_Rect healthRect = {(int)x, Y, 100, 50};
    SDL_Rect lifeRect = {(int)x, 525, 25, 25}; 

    for(int i = 0; i < 4; i++)
    {
        if(flagRenderGet(players[i].render->flags, RENDER_ACTIVE)) 
        {
            snprintf(stringBuffer, sizeof(stringBuffer), "P%d% d%%", i+1, players[i].health);
        
            SDL_Color color = {255, 255-players[i].health, 255- 2*players[i].health};
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
        }
    }
}
