#define BUTTON_X 140
#define BUTTON_Y 210
#define BUTTON_WIDTH 120
#define BUTTON_HEIGHT 40
#define BUTTON_GAP 60

#include "../include/menu.h"


void renderMenu(SDL_Renderer* renderer, SDL_Texture* backgroundTexture, SDL_Rect backgroundRect, MenuButton* buttons)
{
    SDL_RenderCopyEx(renderer, backgroundTexture, NULL, &backgroundRect, 0.0, NULL, 0);

    for(int i = 0; i < 3; i++)
    {
        SDL_RenderCopyEx(renderer, buttons[i].texture, NULL, &buttons[i].rect, 0.0, NULL, 0);
    }
}


void createButtons(SDL_Renderer* renderer, MenuButton* buttons)
{   
    buttons[0].text.string = "Play";
    buttons[0].text.font = TTF_OpenFont("./resources/fonts/arial.ttf", 20);
    buttons[0].text.color = (SDL_Color){255, 255, 255, 155};

    buttons[1].text.string = "Connect";
    buttons[1].text.font = TTF_OpenFont("./resources/fonts/arial.ttf", 20);
    buttons[1].text.color = (SDL_Color){255, 255, 255, 155};

    buttons[2].text.string = "Quit";
    buttons[2].text.font = TTF_OpenFont("./resources/fonts/arial.ttf", 20);
    buttons[2].text.color = (SDL_Color){255, 255, 255, 155};

    for(int i = 0; i < 3; i++)
    {
       buttons[i].rect = (SDL_Rect){BUTTON_X, BUTTON_Y + i * BUTTON_GAP, BUTTON_WIDTH, BUTTON_HEIGHT};
       buttons[i].surface = TTF_RenderText_Solid(buttons[i].text.font, buttons[i].text.string, buttons[i].text.color);
       buttons[i].texture = SDL_CreateTextureFromSurface(renderer, buttons[i].surface);
       SDL_FreeSurface(buttons[i].surface);
    }
}

void renderIngameMenu(SDL_Renderer* renderer, Text menuText, SDL_Rect* buttons)
{
    buttons[0] = (SDL_Rect){BUTTON_X, BUTTON_Y - 100, BUTTON_WIDTH + 400, BUTTON_HEIGHT + 200};
    buttons[1] = (SDL_Rect){BUTTON_X, BUTTON_Y + 100, BUTTON_WIDTH + 400, BUTTON_HEIGHT + 200};

    SDL_Color textColor = {255, 255, 255, 255};

    menuText.font = TTF_OpenFont("./resources/fonts/moiser.ttf", 100);

    //render background
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 60);
    SDL_Rect rect = {0, 0, 800, 600};
    SDL_RenderFillRect(renderer, &rect);

    SDL_Surface* button0 = TTF_RenderText_Solid(menuText.font, "Return To Game", textColor);
    SDL_Texture* textTexture0 = SDL_CreateTextureFromSurface(renderer, button0);

    SDL_Surface* button1 = TTF_RenderText_Solid(menuText.font, "Main Menu", textColor);
    SDL_Texture* textTexture1 = SDL_CreateTextureFromSurface(renderer, button1);

    SDL_RenderCopyEx(renderer, textTexture0, NULL, &buttons[0], 0.0, NULL, 0);
    SDL_RenderCopyEx(renderer, textTexture1, NULL, &buttons[1], 0.0, NULL, 0);

    SDL_FreeSurface(button0);
    SDL_DestroyTexture(textTexture0);
    SDL_FreeSurface(button1);
    SDL_DestroyTexture(textTexture1);
    TTF_CloseFont(menuText.font);
}
