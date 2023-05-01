#define BUTTON_X 140
#define BUTTON_Y 210
#define BUTTON_WIDTH 120
#define BUTTON_HEIGHT 40

#include "../include/menu.h"


SDL_Rect* renderMenu(SDL_Renderer* renderer, Text menuText)
{
    SDL_Rect* buttonRects = malloc(sizeof(SDL_Rect) * 3);
    buttonRects[0] = (SDL_Rect){BUTTON_X, BUTTON_Y, BUTTON_WIDTH, BUTTON_HEIGHT};
    buttonRects[1] = (SDL_Rect){BUTTON_X, BUTTON_Y+60, BUTTON_WIDTH, BUTTON_HEIGHT};
    buttonRects[2] = (SDL_Rect){BUTTON_X, BUTTON_Y+120, BUTTON_WIDTH, BUTTON_HEIGHT};

    SDL_Color textColor = {255, 255, 255, 155};
    menuText.font = TTF_OpenFont("./resources/fonts/LuckiestGuy.ttf", 100);

    //render background
    SDL_Texture* texture = IMG_LoadTexture(renderer, "resources/menu/menu.png");
    SDL_Rect backgroundRect = {0, 0, 800, 600};
    SDL_RenderCopyEx(renderer, texture, NULL, &backgroundRect, 0.0, NULL, 0);
    SDL_DestroyTexture(texture);

    //render button1
    SDL_Surface* button1 = TTF_RenderText_Solid(menuText.font, "Play", textColor);
    texture = SDL_CreateTextureFromSurface(renderer, button1);
    SDL_RenderCopyEx(renderer, texture, NULL, &buttonRects[0], 0.0, NULL, 0);
    SDL_FreeSurface(button1);
    SDL_DestroyTexture(texture);

    //render button2
    SDL_Surface* button2 = TTF_RenderText_Solid(menuText.font, "EMPTY", textColor);
    texture = SDL_CreateTextureFromSurface(renderer, button2);
    SDL_RenderCopyEx(renderer, texture, NULL, &buttonRects[1], 0.0, NULL, 0);
    SDL_FreeSurface(button2);
    SDL_DestroyTexture(texture);

    //render button3
    SDL_Surface* button3 = TTF_RenderText_Solid(menuText.font, "Quit", textColor);
    texture = SDL_CreateTextureFromSurface(renderer, button3);
    SDL_RenderCopyEx(renderer, texture, NULL, &buttonRects[2], 0.0, NULL, 0);
    SDL_FreeSurface(button3);
    SDL_DestroyTexture(texture);

    TTF_CloseFont(menuText.font);
    return buttonRects;
}

