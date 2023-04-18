#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "../include/vec2.h"
#include "../include/physicsObject.h"
#include "../include/renderObject.h"
#include "../include/keyboard.h"
#include "../include/player.h"
#include "../include/spriteHandler.h"
#define GRAVITY 9.82f //9.82f

int isColliding(SDL_Rect rect1, SDL_Rect rect2)
{
    SDL_bool intersect = SDL_HasIntersection(&rect1, &rect2);
    if (intersect == SDL_TRUE)
        return 1;
    return 0;
}

int main(int argv, char **args)
{
    SDL_Init(SDL_INIT_EVERYTHING);

    SDL_Window *window = SDL_CreateWindow("Hello SDL", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, 0);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);

    int isRunning = 1;
    SDL_Event event;

    // Player player;
    // Player player2;

    Player players[2];

    RenderObject objects[4];
    int amountOfObjects = 4;

    //   Render order: start at 0 continue up.
    objects[0].order = 0;
    objects[0].texture = IMG_LoadTexture(renderer, "resources/background.png");
    objects[0].imageExtents = (SDL_Rect){0, 0, 3000, 2000};
    objects[0].screenExtents = (SDL_Rect){0, 0, 800, 600};
    objects[0].flip = 0;

    objects[1].order = 1;
    objects[1].texture = IMG_LoadTexture(renderer, "resources/platform.png");
    objects[1].imageExtents = (SDL_Rect){0, 0, 1200, 1200};
    objects[1].screenExtents = (SDL_Rect){100, 300, 600, 150};
    objects[1].flip = 0;

    objects[2].order = 2;
    objects[2].texture = IMG_LoadTexture(renderer, "resources/stickmanSprite.png");
    objects[2].imageExtents = (SDL_Rect){32, 0, 32, 64};
    objects[2].screenExtents = (SDL_Rect){400, 300, 32, 64};
    objects[2].flip = 0;

    objects[3].order = 2;
    objects[3].texture = IMG_LoadTexture(renderer, "resources/stickmanSprite2.png");
    objects[3].imageExtents = (SDL_Rect){32, 0, 32, 64};
    objects[3].screenExtents = (SDL_Rect){400, 300, 32, 64};
    objects[3].sprites = 3;
    objects[3].flip = 0;

    players[0].render = &objects[2];
    players[1].render = &objects[3];

    //  two objects for now...
    PhysicsObject physicsObjects[2] = {{{0.f, 0.f}, {0.f, 0.f}, {0.f, 0.f}, 0}};

    players[0].physics = &physicsObjects[0];
    players[1].physics = &physicsObjects[1];

    physicsObjects[0].position = vec2(400.0f, 200.0f);
    physicsObjects[1].position = vec2(400.0f, 200.0f);

    KeyboardStates states = {0};

    int frameCounter = 0;
    while (isRunning)
    {
        //player.physics->acceleration.y = 9.82f;
        for(int x = 2; x < 4; x++)
        {
            if (isColliding(objects[x].screenExtents, objects[1].screenExtents))
            {
                players[x - 2].physics->velocity.y = 0.0f;
                players[x - 2].physics->acceleration.y = 0;
            }
            else
                players[x - 2].physics->acceleration.y = GRAVITY;
        }

        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_QUIT:
                isRunning = 0;
                break;
            case SDL_KEYDOWN:
            case SDL_KEYUP:
                handleKeyboardInputs(&states, event.key.keysym.sym, event.type);
                break;
            }
        }

        if (states.keyState[SDLK_ESCAPE])
        {
            isRunning = 0;
        }
        if (states.keyState[SDLK_a])
        {
            players[0].render->flip = 1;
            players[0].physics->velocity.x -= 1.0f;
            if (players[0].physics->velocity.x < -10.0f)
            {
                players[0].physics->velocity.x = -10.0f;
            }
            if (objects[2].imageExtents.x == spriteHandler(objects[2],1))
            {
                objects[2].imageExtents.x = spriteHandler(objects[2],2);
            }
            else
            {
                objects[2].imageExtents.x = spriteHandler(objects[2],1);
            }
        }
        if (states.keyState[SDLK_d])
        {
            players[0].render->flip = 0;
            players[0].physics->velocity.x += 1.0f;
            if (players[0].physics->velocity.x > 10.0f)
            {
                players[0].physics->velocity.x = 10.0f;
            }
            if (objects[2].imageExtents.x == spriteHandler(objects[2],1))
            {
                objects[2].imageExtents.x = spriteHandler(objects[2],2);
            }
            else
            {
                objects[2].imageExtents.x = spriteHandler(objects[2],1);
            }
        }

        if (states.keyState[SDLK_j])
        {
            players[1].render->flip = 1;
            players[1].physics->velocity.x -= 1.0f;
            if (players[1].physics->velocity.x < -10.0f)
            {
                players[1].physics->velocity.x = -10.0f;
            }
            if (objects[3].imageExtents.x == spriteHandler(objects[3],1))
            {
                objects[3].imageExtents.x = spriteHandler(objects[3],2);
            }
            else
            {
                objects[3].imageExtents.x = spriteHandler(objects[3],1);
            }
        }
        if (states.keyState[SDLK_l])
        {
            players[1].render->flip = 0;
            players[1].physics->velocity.x += 1.0f;
            if (players[1].physics->velocity.x > 10.0f)
            {
                players[1].physics->velocity.x = 10.0f;
            }
            if (objects[3].imageExtents.x == spriteHandler(objects[3],1))
            {
                objects[3].imageExtents.x = spriteHandler(objects[3],2);
            }
            else
            {
                objects[3].imageExtents.x = spriteHandler(objects[3],1);
            }
        }

        //  we should fix a function to updatePosition of all objects like the function renderObjects 
        updatePosition(players[0].physics, 1.0f / 60.0f);
        players[0].render->screenExtents.x = (int)players[0].physics->position.x;
        players[0].render->screenExtents.y = (int)players[0].physics->position.y;

        updatePosition(players[1].physics, 1.0f / 60.0f);
        players[1].render->screenExtents.x = (int)players[1].physics->position.x;
        players[1].render->screenExtents.y = (int)players[1].physics->position.y;

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