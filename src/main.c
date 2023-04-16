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
#define GRAVITY 0.25f //9.82f

int checkCollision(SDL_Rect rect1, SDL_Rect rect2)
{
    SDL_bool intersect = SDL_HasIntersection(&rect1, &rect2);
    if (intersect == SDL_TRUE)
        return 1;
    else
        printf("no intersection\n");
        return 0;
}

int main(int argv, char **args)
{
    SDL_Init(SDL_INIT_EVERYTHING);

    SDL_Window *window = SDL_CreateWindow("Hello SDL", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, 0);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);

    int isRunning = 1;
    SDL_Event event;

    Player player;
    Player player2;
    RenderObject objects[4];
    int amountOfObjects = 4;

    //   ugly animation code made by William
    int swapAnimation = 0; //  <-- is this needed? --Damien

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
    objects[3].flip = 0;

    
    player.render = &objects[2];
    player2.render = &objects[3];

    //  two objects for now...
    PhysicsObject physicsObjects[2] = {{{0.f, 0.f}, {0.f, 0.f}, {0.f, 0.f}, 0}};

    player.physics = &physicsObjects[0];
    player2.physics = &physicsObjects[1];

    physicsObjects[0].position = vec2(400.0f, 200.0f);
    physicsObjects[1].position = vec2(400.0f, 200.0f);

    KeyboardStates states = {0};

    int frameCounter = 0;
    while (isRunning)
    {
        //player.physics->acceleration.y = 9.82f;
        
        if (checkCollision(objects[2].screenExtents, objects[1].screenExtents))
        {
            printf("Player 1 collides with platform\n");
        }

        player.physics->acceleration.y = GRAVITY;
        player2.physics->acceleration.y = GRAVITY;

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
            player.render->flip = 1;
            player.physics->velocity.x -= 1.0f;
            if (player.physics->velocity.x < -10.0f)
            {
                player.physics->velocity.x = -10.0f;
            }
            if (objects[2].imageExtents.x == 32)
            {
                objects[2].imageExtents.x = 64;
            }
            else
            {
                objects[2].imageExtents.x = 32;
            }
        }
        if (states.keyState[SDLK_d])
        {
            player.render->flip = 0;
            player.physics->velocity.x += 1.0f;
            if (player.physics->velocity.x > 10.0f)
            {
                player.physics->velocity.x = 10.0f;
            }
            if (objects[2].imageExtents.x == 32)
            {
                objects[2].imageExtents.x = 64;
            }
            else
            {
                objects[2].imageExtents.x = 32;
            }
        }

        if (states.keyState[SDLK_j])
        {
            player2.render->flip = 1;
            player2.physics->velocity.x -= 1.0f;
            if (player2.physics->velocity.x < -10.0f)
            {
                player2.physics->velocity.x = -10.0f;
            }
            if (objects[3].imageExtents.x == 32)
            {
                objects[3].imageExtents.x = 64;
            }
            else
            {
                objects[3].imageExtents.x = 32;
            }
        }
        if (states.keyState[SDLK_l])
        {
            player2.render->flip = 0;
            player2.physics->velocity.x += 1.0f;
            if (player2.physics->velocity.x > 10.0f)
            {
                player2.physics->velocity.x = 10.0f;
            }
            if (objects[3].imageExtents.x == 32)
            {
                objects[3].imageExtents.x = 64;
            }
            else
            {
                objects[3].imageExtents.x = 32;
            }
        }

        //  we should fix a function to updatePosition of all objects like the function renderObjects
        updatePosition(player.physics, 1.0f / 60.0f);

        player.render->screenExtents.x = (int)player.physics->position.x;
        player.render->screenExtents.y = (int)player.physics->position.y;

        updatePosition(player2.physics, 1.0f / 60.0f);
        player2.render->screenExtents.x = (int)player2.physics->position.x;
        player2.render->screenExtents.y = (int)player2.physics->position.y;

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