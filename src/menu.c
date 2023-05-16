#include "../include/menu.h"

#define BUTTON_X 140
#define BUTTON_Y 210
#define BUTTON_WIDTH 120
#define BUTTON_HEIGHT 40
#define BUTTON_GAP 60

void renderMenu(SDL_Renderer* renderer, SDL_Texture* backgroundTexture, SDL_Rect backgroundRect, MenuButton* buttons)
{
    SDL_RenderCopyEx(renderer, backgroundTexture, NULL, &backgroundRect, 0.0, NULL, 0);

    for(int i = 0; i < 3; i++)
    {
        SDL_RenderCopyEx(renderer, buttons[i].texture, NULL, &buttons[i].rect, 0.0, NULL, 0);
    }
}

void createButtons(SDL_Renderer* renderer, MenuButton* buttons, TTF_Font* font)
{   
    buttons[0].text.string = "Play";
    buttons[0].text.color = (SDL_Color){255, 255, 255, 155};
    buttons[0].rect = (SDL_Rect){BUTTON_X, BUTTON_Y, BUTTON_WIDTH, BUTTON_HEIGHT};

    buttons[1].text.string = "Connect";
    buttons[1].text.color = (SDL_Color){255, 255, 255, 155};
    buttons[1].rect = (SDL_Rect){BUTTON_X, BUTTON_Y + BUTTON_GAP, BUTTON_WIDTH, BUTTON_HEIGHT};

    buttons[2].text.string = "Quit";
    buttons[2].text.color = (SDL_Color){255, 255, 255, 155};
    buttons[2].rect = (SDL_Rect){BUTTON_X, BUTTON_Y + 2 * BUTTON_GAP, BUTTON_WIDTH, BUTTON_HEIGHT};

    buttons[3].text.string = "Return to Game";
    buttons[3].text.color = (SDL_Color){255, 255, 255, 255};
    buttons[3].rect = (SDL_Rect){BUTTON_X, BUTTON_Y - 100, BUTTON_WIDTH + 400, BUTTON_HEIGHT + 200};

    buttons[4].text.string = "Main Menu";
    buttons[4].text.color = (SDL_Color){255, 255, 255, 255};
    buttons[4].rect = (SDL_Rect){BUTTON_X, BUTTON_Y + 100, BUTTON_WIDTH + 400, BUTTON_HEIGHT + 200};

    for(int i = 0; i < 5; i++)
    {
        buttons[i].surface = TTF_RenderText_Solid(font, buttons[i].text.string, buttons[i].text.color);
        buttons[i].texture = SDL_CreateTextureFromSurface(renderer, buttons[i].surface);
        SDL_FreeSurface(buttons[i].surface);
    }
}

void renderIngameMenu(SDL_Renderer* renderer, SDL_Rect backgroundRect, MenuButton* buttons)
{
    
    //render background
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 60);
    SDL_RenderFillRect(renderer, &backgroundRect);

    SDL_RenderCopyEx(renderer, buttons[3].texture, NULL, &buttons[3].rect, 0.0, NULL, 0);
    SDL_RenderCopyEx(renderer, buttons[4].texture, NULL, &buttons[4].rect, 0.0, NULL, 0);
}

void freeButtons(MenuButton* buttons, int amountOfButtons)
{
    for(int i = 0; i < amountOfButtons; i++)
    {
        SDL_DestroyTexture(buttons[i].texture);
    }
}
