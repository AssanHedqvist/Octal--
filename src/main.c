#include <stdio.h>
#include <math.h>
#include <stdlib.h> 
#include <time.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "../include/vec2.h"
#include "../include/physicsObject.h"
#include "../include/renderObject.h"

int main(int argv, char** args) 
{     

    SDL_Init(SDL_INIT_EVERYTHING);

    SDL_Window *window = SDL_CreateWindow("Hello SDL", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, 0);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);

    int isRunning = 1;
    SDL_Event event;

    RenderObject objects[3];
    int amountOfObjects = 3;

    objects[2].texture = IMG_LoadTexture(renderer,"resources/haha.png");
    objects[2].imageExtents = (SDL_Rect){0,0,334,340};
    objects[2].screenExtents = (SDL_Rect){100,364,600,50};
    objects[2].order = 2;

    objects[1].texture = IMG_LoadTexture(renderer,"resources/background.png");
    objects[1].imageExtents = (SDL_Rect){0,0,3000,2000};
    objects[1].screenExtents = (SDL_Rect){0,0,800,600};
    objects[1].order = 0;

    objects[0].texture = IMG_LoadTexture(renderer,"resources/stickmanSprite.png");
    objects[0].imageExtents = (SDL_Rect){32,0,32,64};
    objects[0].screenExtents = (SDL_Rect){400,300,32,64};
    objects[0].order = 1;

    for (int i = 0; i < amountOfObjects; i++)
    {
        objects[i].flip = 0;
    }

    int frameCounter = 0;
    while (isRunning)
    {   
        //  this code can be removed later
        if((frameCounter % 16) == 0) 
        {
            objects[0].screenExtents.x -= 1;
        }

        //  this code can be removed later
        if(objects[0].screenExtents.x == 800) 
        {
            objects[0].screenExtents.x = -31;
        }
       
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
                case SDL_QUIT:
                    isRunning = 0;
                    break;
                case SDL_KEYDOWN:
                    if (event.key.keysym.sym == SDLK_ESCAPE)
                    {
                        isRunning = 0;
                    }
                    break;
                case SDL_KEYUP:
                    
                    break;
            }
        }

        renderObjects(renderer, objects, amountOfObjects);

        frameCounter++;
    }

    for (int i = 0; i < amountOfObjects; i++)
    {
        SDL_DestroyTexture(objects[i].texture);
    }
    
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}