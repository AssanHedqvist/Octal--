#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_net.h>
#include <SDL2/SDL_ttf.h>
#include "../include/vec2.h"
#include "../include/physicsObject.h"
#include "../include/renderObject.h"
#include "../include/keyboard.h"
#include "../include/player.h"
#include "../include/text.h"
#include "../include/spriteHandler.h"
#include "../include/attacks.h"

//  bad function name --Damien
void updateRenderWithPhysics(RenderObject render[], PhysicsObject physics[], int lengthOfPhysics)
{
    for (int i = 0; i < lengthOfPhysics; i++)
    {
        //  just 1 for now because the difference is 1 right now
        render[i + 1].screenExtents.x = (int)physics[i].pos.x;
        render[i + 1].screenExtents.y = (int)(600.0f - (physics[i].pos.y + physics[i].extents.y));
    }
}

#define SUB_STEPS 4
#define DT (1.0f / (60.0f * (float)SUB_STEPS))

enum GameStates
{
    MENU,
    RUNNING,
    CLOSED
};

int main(int argv, char **args)
{
    UDPsocket sd;
    IPaddress srvadd;
    UDPpacket *toServer;
    UDPpacket *fromServer;
    Player players[4] = {0, 0, {0}, 0, 0, 0};
    Text playerHealthText[4];

    SDL_Init(SDL_INIT_EVERYTHING);

    if (TTF_Init() < 0)
    {
        exit(EXIT_FAILURE);
    }

    if (SDLNet_Init() < 0)
    {
        // fprintf(stderr, "SDLNet_Init: %s\n", SDLNet_GetError());
        exit(EXIT_FAILURE);
    }

    //   open socket
    if (!(sd = SDLNet_UDP_Open(0)))
    {
        // fprintf(stderr, "SDLNet_UDP_Open: %s\n", SDLNet_GetError());
        exit(EXIT_FAILURE);
    }

    //   Resolve server name
    if (SDLNet_ResolveHost(&srvadd, "127.0.0.1", 15661) == -1)
    {
        // fprintf(stderr, "SDLNet_ResolveHost(192.0.0.1 2000): %s\n", SDLNet_GetError());
        exit(EXIT_FAILURE);
    }

    //   Allocates space for packet
    if (!((toServer = SDLNet_AllocPacket(250)) && (fromServer = SDLNet_AllocPacket(250))))
    {
        // fprintf(stderr, "SDLNet_AllocPacket: %s\n", SDLNet_GetError());
        exit(EXIT_FAILURE);
    }

    // printf("%u.%u.%u.%u\n", address->host & 0xFF, (( address->host >> 8) & 0xFF),  (( address->host >> 16) & 0xFF), ((address->host >> 24) & 0xFF));

    //  server connecting code

    toServer->address.host = srvadd.host;
    toServer->address.port = srvadd.port;

    int tmp = 1;

    memmove(toServer->data, (void *)&tmp, 4);
    toServer->len = 4;
    SDLNet_UDP_Send(sd, -1, toServer);

    int thisComputersPlayerIndex = 0;

    while (thisComputersPlayerIndex == -1)
    {
        if (SDLNet_UDP_Recv(sd, fromServer) == 1)
        {
            memmove((void *)&thisComputersPlayerIndex, fromServer->data, 4);
        }
    }

    printf("Client: %d\n", thisComputersPlayerIndex);

    //  server connecting code

    SDL_Window *window = SDL_CreateWindow("Hello Octal--!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_ALWAYS_ON_TOP);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);

    int isRunning = 1;
    SDL_Event event;

    int amountOfRenderObjects = 6;
    RenderObject renderObjects[6];

    //   Render order: start at 0 continue up.
    renderObjects[0].order = 0;
    renderObjects[0].texture = IMG_LoadTexture(renderer, "resources/background.png");
    renderObjects[0].imageExtents = (SDL_Rect){0, 0, 6000, 3000};
    renderObjects[0].screenExtents = (SDL_Rect){0, 0, 800, 600};
    renderObjects[0].flip = 0;

    renderObjects[1].order = 1;
    renderObjects[1].texture = IMG_LoadTexture(renderer, "resources/platform.png");
    renderObjects[1].imageExtents = (SDL_Rect){0, 0, 1054, 289};
    renderObjects[1].screenExtents = (SDL_Rect){100, 300, 600, 150};
    renderObjects[1].flip = 0;

    renderObjects[2].order = 2;
    renderObjects[2].texture = IMG_LoadTexture(renderer, "resources/stickmanSprite.png");
    renderObjects[2].imageExtents = (SDL_Rect){0, 0, 32, 64};
    renderObjects[2].screenExtents = (SDL_Rect){400, 300, 32, 64};
    renderObjects[2].flip = 0;

    renderObjects[3].order = 2;
    renderObjects[3].texture = IMG_LoadTexture(renderer, "resources/stickman/stickmanSprite2.png");
    renderObjects[3].imageExtents = (SDL_Rect){0, 0, 32, 64};
    renderObjects[3].screenExtents = (SDL_Rect){300, 300, 32, 64};
    renderObjects[3].flip = 0;

    renderObjects[4].order = 2;
    renderObjects[4].texture = IMG_LoadTexture(renderer, "resources/stickman/stickmanSprite3.png");
    renderObjects[4].imageExtents = (SDL_Rect){0, 0, 32, 64};
    renderObjects[4].screenExtents = (SDL_Rect){300, 300, 32, 64};
    renderObjects[4].flip = 0;

    renderObjects[5].order = 2;
    renderObjects[5].texture = IMG_LoadTexture(renderer, "resources/stickman/stickmanSprite4.png");
    renderObjects[5].imageExtents = (SDL_Rect){0, 0, 32, 64};
    renderObjects[5].screenExtents = (SDL_Rect){300, 300, 32, 64};
    renderObjects[5].flip = 0;

    players[0].render = &renderObjects[2];
    players[1].render = &renderObjects[3];
    players[2].render = &renderObjects[4];
    players[3].render = &renderObjects[5];

    //  three renderObjects for now...
    int amountOfPhysicalObjects = 5;
    PhysicsObject physicsObjects[5] = {{{0.f, 0.f}, {0.f, 0.f}, {0.f, 0.f}, {0.f, 0.f}, 0}};

    /*
    Damien:
    I am going to make it so that you won't need to set the oldPos yourself
    that is because im going to make it so it can be calculated
    */

    //  (platform)
    physicsObjects[0].acceleration = vec2(0.f, 0.f);
    physicsObjects[0].pos = vec2(100, 150);
    physicsObjects[0].oldPos = physicsObjects[0].pos;
    physicsObjects[0].extents = vec2(600, 150);
    physicsObjects[0].type = STATIC;

    //  (player 0)
    physicsObjects[1].acceleration = vec2(0.f, -982.0f);
    physicsObjects[1].pos = vec2(150, 536);
    physicsObjects[1].oldPos = physicsObjects[1].pos;
    physicsObjects[1].extents = vec2(32, 64);
    physicsObjects[1].type = DYNAMIC;
    //  (player 1)
    physicsObjects[2].acceleration = vec2(0.f, -982.0f);
    physicsObjects[2].pos = vec2(316.6666666666666666666666666666f, 536);
    physicsObjects[2].oldPos = physicsObjects[2].pos;
    physicsObjects[2].extents = vec2(32, 64);
    physicsObjects[2].type = DYNAMIC;

    //  (player 2)
    physicsObjects[3].acceleration = vec2(0.f, -982.0f);
    physicsObjects[3].pos = vec2(483.33333333333333333333333333334f, 536);
    physicsObjects[3].oldPos = physicsObjects[3].pos;
    physicsObjects[3].extents = vec2(32, 64);
    physicsObjects[3].type = DYNAMIC;

    //  (player 3)
    physicsObjects[4].acceleration = vec2(0.f, -982.0f);
    physicsObjects[4].pos = vec2(650, 536);
    physicsObjects[4].oldPos = physicsObjects[4].pos;
    physicsObjects[4].extents = vec2(32, 64);
    physicsObjects[4].type = DYNAMIC;

    players[0].physics = &physicsObjects[1];
    players[1].physics = &physicsObjects[2];
    players[2].physics = &physicsObjects[3];
    players[3].physics = &physicsObjects[4];

    players[0].amountOfJumpsLeft = 2;
    players[1].amountOfJumpsLeft = 2;

    int frameCounter = 0;

    players[0].health = 0;
    players[1].health = 0;
    players[2].health = 0;
    players[3].health = 0;

    playerHealthText->font = TTF_OpenFont("./resources/fonts/arial.ttf", 20);

    struct timespec t1, t2;
  
    int currentGameState = MENU;
    while (currentGameState != CLOSED)
    {
        switch (currentGameState)
        {
            case MENU:
            {
                SDL_Rect backRect = {0,0,800,600};
                SDL_Rect textRect = {50, 250, 700, 100};
                Text MenuText;
                MenuText.font = TTF_OpenFont("./resources/fonts/arial.ttf", 20);

                SDL_Texture* texture = IMG_LoadTexture(renderer, "resources/menu/menu.png");
                SDL_RenderCopyEx(renderer, texture, NULL, &backRect, 0.0, NULL, 0);
                SDL_DestroyTexture(texture);

                SDL_Color textColor = {255, 255, 255, 155};
                SDL_Surface* menuMessage = TTF_RenderText_Solid(MenuText.font, "Press Space to start game, Esc to exit", textColor);
                texture = SDL_CreateTextureFromSurface(renderer, menuMessage);
                SDL_RenderCopyEx(renderer, texture, NULL, &textRect, 0.0, NULL, 0);
                SDL_FreeSurface(menuMessage);
                SDL_DestroyTexture(texture);

                while(SDL_PollEvent(&event))
                {   
                    //we probably want it to handle mouseinputs rather
                    switch(event.type)
                    {
                        case SDL_QUIT:
                            currentGameState = CLOSED;
                            break;
                        case SDL_KEYDOWN:
                            //Behöver fråga Damien om "IsKeyDown och HandleKeyboardInputs"
                            switch(event.key.keysym.sym)
                            {
                                case SDLK_SPACE:
                                    currentGameState = RUNNING;
                                    break;
                                case SDLK_ESCAPE:
                                    currentGameState = CLOSED;
                                    break;
                            }
                            break;
                        default:
                            break;
                    }
                }
                SDL_RenderPresent(renderer);
                break;
            }
            case RUNNING:
            {
                clock_gettime(CLOCK_MONOTONIC, &t1);
                while (SDL_PollEvent(&event))
                {
                    switch (event.type)
                    {
                        case SDL_QUIT:
                            handleKeyboardInputs(&players[0].keyInputs, SDL_SCANCODE_ESCAPE, SDL_KEYDOWN);
                            break;
                        case SDL_KEYDOWN:
                        case SDL_KEYUP:
                            handleKeyboardInputs(&players[0].keyInputs, event.key.keysym.scancode, event.type);
                            break;
                    }
                }   

                memmove(toServer->data, (void *)&players[0].keyInputs.keyState, 32);
                toServer->len = 32;

                SDLNet_UDP_Send(sd, -1, toServer);

                if (isKeyDown(&players[0].keyInputs, SDL_SCANCODE_E))
                {
                    players[0].health += 10;
                }
                if (isKeyDown(&players[0].keyInputs, SDL_SCANCODE_ESCAPE))
                {
                    currentGameState = CLOSED;
                }

                handlePlayerAnimation(players);
                handlePlayerInputs(&players[0], DT);
                handlePlayerLives(&players[0]); //  did (&players) work for anybody? -- Damien
                lightPunch(players, 4);         //  did (&players) work for anybody? -- Damien

                for (int i = 0; i < amountOfPhysicalObjects; i++)
                {
                    physicsObjects[i].recentCollision = 0;
                }

                for (int i = 0; i < SUB_STEPS; i++)
                {
                    for (int i = 0; i < 4; i++)
                    {
                        // collision detection with windows boundries
                        if (players[i].physics->pos.x <= 0.f)
                        {
                            // if you collide reset the position so u dont go out of the screen
                            players[i].physics->pos.x = 0.f;
                        }
                        if (players[i].physics->pos.y <= 0.f)
                        {
                            players[i].physics->pos.y = 0.f;
                        }
                        // subtract the width of the sprite.
                        if (players[i].physics->pos.x + players[i].physics->extents.x >= 800.0f)
                        {
                            players[i].physics->pos.x = 800.0f - players[i].physics->extents.x;
                        }
                        // subtract the height of the sprite.
                        if (players[i].physics->pos.y + players[i].physics->extents.y >= 600.0f)
                        {
                            players[i].physics->pos.y = 600.0f - players[i].physics->extents.y;
                        }
                    }

                    constraintSolve(physicsObjects, amountOfPhysicalObjects);
                    updatePositions(physicsObjects, amountOfPhysicalObjects, DT);
                }

            // //   Receive data
            // if (SDLNet_UDP_Recv(sd, fromServer))
            // {
            //     // float a, b;
            //     int b;
            //     vec2 a;
            //     sscanf((char *)fromServer->data, "%f %f %d\n", &a.x, &a.y, &b);
            //     // printf("RECIEVED %f  %f\n", a, b);
            //     // players[0].physics->pos.x = a;
            //     // players[0].physics->pos.y = b;
            //     players[otherPlayer].physics->pos = a;
            //     players[otherPlayer].render->flip = b;
            // }

                updateRenderWithPhysics(renderObjects, physicsObjects, amountOfPhysicalObjects);

                render(renderer, renderObjects, amountOfRenderObjects);
                renderPlayerHealth(players, 4, renderer, playerHealthText->font, 100, 550);
                SDL_RenderPresent(renderer);

                frameCounter++;

                //  16638935 = (1/60.1) * 1000000000
                t1.tv_sec += ((t1.tv_nsec + 16638935) / 1000000000);
                t1.tv_nsec = ((t1.tv_nsec + 16638935) % 1000000000);

                do
                {
                    clock_gettime(CLOCK_MONOTONIC, &t2);
                    //  while we have not passed 16ms in one frame wait till that has happened
                } while (((t2.tv_sec < t1.tv_sec) || ((t2.tv_sec == t1.tv_sec) && (t2.tv_nsec < t1.tv_nsec))));
                break;
            }
        }
    }

    TTF_Quit();

    SDLNet_Quit();
    SDLNet_FreePacket(toServer);
    SDLNet_FreePacket(fromServer);

    for (int i = 0; i < amountOfRenderObjects; i++)
    {
        SDL_DestroyTexture(renderObjects[i].texture);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}