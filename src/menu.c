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

    buttons[1].text.string = "Connect";
    buttons[1].text.font = TTF_OpenFont("./resources/fonts/arial.ttf", 20);

    buttons[2].text.string = "Quit";
    buttons[2].text.font = TTF_OpenFont("./resources/fonts/arial.ttf", 20);
    
    SDL_Color textColor = {255, 255, 255, 155};     

    for(int i = 0; i < 3; i++)
    {
       buttons[i].rect = (SDL_Rect){BUTTON_X, BUTTON_Y + i * BUTTON_GAP, BUTTON_WIDTH, BUTTON_HEIGHT};
       buttons[i].surface = TTF_RenderText_Solid(buttons[i].text.font, buttons[i].text.string, textColor);
       buttons[i].texture = SDL_CreateTextureFromSurface(renderer, buttons[i].surface);
       SDL_FreeSurface(buttons[i].surface);
    }
}

