#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_net.h>
#include "../include/vec2.h"
#include "../include/physicsObject.h"
#include "../include/renderObject.h"
#include "../include/keyboard.h"
#include "../include/player.h"
#include "../include/spriteHandler.h"

//  bad function name --Damien
void convertToRenders(RenderObject render[], PhysicsObject physics[], int lengthOfPhysics)
{
    for (int i = 0; i < lengthOfPhysics; i++)
    {
        //  just 1 for now because the difference is 1 right now
        render[i + 1].screenExtents.x = (int)physics[i].pos.x;
        render[i + 1].screenExtents.y = (int)(600.0f - (physics[i].pos.y + physics[i].extents.y));
    }
}

int main(int argv, char **args)
{

    UDPsocket sd;
    IPaddress srvadd;
    UDPpacket *player1;
    UDPpacket *player2;

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
    if (SDLNet_ResolveHost(&srvadd, "127.0.0.1", 2000) == -1)
    {
        // fprintf(stderr, "SDLNet_ResolveHost(192.0.0.1 2000): %s\n", SDLNet_GetError());
        exit(EXIT_FAILURE);
    }
    //   Allocates space for packet
    if (!((player1 = SDLNet_AllocPacket(144)) && (player2 = SDLNet_AllocPacket(512))))
    {
        // fprintf(stderr, "SDLNet_AllocPacket: %s\n", SDLNet_GetError());
        exit(EXIT_FAILURE);
    }

    SDL_Init(SDL_INIT_EVERYTHING);

    SDL_Window *window = SDL_CreateWindow("Hello Octal--!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, 0);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);

    int isRunning = 1;
    SDL_Event event;

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
    objects[1].imageExtents = (SDL_Rect){0, 0, 1054, 289};
    objects[1].screenExtents = (SDL_Rect){100, 300, 600, 150};
    objects[1].flip = 0;

    objects[2].order = 2;
    objects[2].texture = IMG_LoadTexture(renderer, "resources/stickmanSprite.png");
    objects[2].imageExtents = (SDL_Rect){0, 0, 32, 64};
    objects[2].screenExtents = (SDL_Rect){400, 300, 32, 64};
    objects[2].flip = 0;

    objects[3].order = 2;
    objects[3].texture = IMG_LoadTexture(renderer, "resources/stickmanSprite2.png");
    objects[3].imageExtents = (SDL_Rect){0, 0, 32, 64};
    objects[3].screenExtents = (SDL_Rect){400, 300, 32, 64};
    objects[3].flip = 0;

    players[0].render = &objects[2];
    players[1].render = &objects[3];

    //  three objects for now...
    int amountOfPhysicalObjects = 3;
    PhysicsObject physicsObjects[3] = {{{0.f, 0.f}, {0.f, 0.f}, {0.f, 0.f}, {0.f, 0.f}, 0}};

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
    physicsObjects[1].pos = vec2(400, 536);
    physicsObjects[1].oldPos = physicsObjects[1].pos;
    physicsObjects[1].extents = vec2(32, 64);
    physicsObjects[1].type = DYNAMIC;

    //  (player 1)
    physicsObjects[2].acceleration = vec2(0.f, -982.0f);
    physicsObjects[2].pos = vec2(300, 536);
    physicsObjects[2].oldPos = physicsObjects[2].pos;
    physicsObjects[2].extents = vec2(32, 64);
    physicsObjects[2].type = DYNAMIC;

    players[0].physics = &physicsObjects[1];
    players[1].physics = &physicsObjects[2];

    players[0].amountOfJumpsLeft = 2;
    players[1].amountOfJumpsLeft = 2;

    KeyboardStates states = {0};

    int frameCounter = 0;

    struct timespec t1, t2;
    while (isRunning)
    {
        clock_gettime(CLOCK_MONOTONIC, &t1);

        t1.tv_sec += ((t1.tv_nsec + 16638935) / 1000000000);
        t1.tv_nsec = ((t1.tv_nsec + 16638935) % 1000000000);

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
            physicsObjects[1].oldPos = vdiff(physicsObjects[1].oldPos, vec2(-1.0, 0.0));
            if (objects[2].imageExtents.x == spriteHandler(objects[2], 1))
            {
                objects[2].imageExtents.x = spriteHandler(objects[2], 2);
            }
            else
            {
                objects[2].imageExtents.x = spriteHandler(objects[2], 1);
            }
        }

        if (states.keyState[SDLK_d])
        {
            players[0].render->flip = 0;
            physicsObjects[1].oldPos = vdiff(physicsObjects[1].oldPos, vec2(1.0, 0.0));
            if (objects[2].imageExtents.x == spriteHandler(objects[2], 1))
            {
                objects[2].imageExtents.x = spriteHandler(objects[2], 2);
            }
            else
            {
                objects[2].imageExtents.x = spriteHandler(objects[2], 1);
            }
        }

        if (states.keyState[SDLK_w])
        {
            physicsObjects[1].oldPos = vdiff(physicsObjects[1].oldPos, vec2(0.0, 1.0));
        }

        if (states.keyState[SDLK_j])
        {
            players[1].render->flip = 1;
            physicsObjects[2].oldPos = vdiff(physicsObjects[2].oldPos, vec2(-1.0, 0.0));
            if (objects[3].imageExtents.x == spriteHandler(objects[3], 1))
            {
                objects[3].imageExtents.x = spriteHandler(objects[3], 2);
            }
            else
            {
                objects[3].imageExtents.x = spriteHandler(objects[3], 1);
            }
        }

        if (states.keyState[SDLK_l])
        {
            players[1].render->flip = 0;
            physicsObjects[2].oldPos = vdiff(physicsObjects[2].oldPos, vec2(1.0, 0.0));
            if (objects[3].imageExtents.x == spriteHandler(objects[3], 1))
            {
                objects[3].imageExtents.x = spriteHandler(objects[3], 2);
            }
            else
            {
                objects[3].imageExtents.x = spriteHandler(objects[3], 1);
            }
        }

        if (states.keyState[SDLK_i])
        {
            physicsObjects[2].oldPos = vdiff(physicsObjects[2].oldPos, vec2(0.0, 1.0));
        }

        for (int i = 0; i < 4; i++)
        {
            constraintSolve(physicsObjects, 3);
            updatePositions(physicsObjects, 3, 1.0f / 240.0f);
        }

        //   Send and retrive positions
        // if (players[0].physics->oldPos.x != players[0].physics->pos.x || players[0].physics->oldPos.y != players[0].render->screenExtents.y)
        printf("%d %d\n", (int)players[0].physics->pos.x, (int)players[0].physics->pos.y);
        //sprintf((char *)player1->data, "%d %d\n", (int)players[0].physics->pos, (int)players[0].render->screenExtents.y);
        memcpy(player1->data, (void*)&players[0], 144);

        player1->address.host = srvadd.host; /* Set the destination host */
        player1->address.port = srvadd.port; /* And destination port */
        player1->len = strlen((char *)player1->data) + 1;

        SDLNet_UDP_Send(sd, -1, player1);

        //   Receive data
       /* if (SDLNet_UDP_Recv(sd, player2))
        {
            int a, b;
            sscanf((char *)player2->data, "%f %d\n", &a, &b);
            players[1].physics->pos = a;
            players[1].physics->pos = b;
            printf("UDP Packet incoming %d %f\n", players[1].physics->pos.x, players[1].physics->pos.y);
        }*/

        

        convertToRenders(objects, physicsObjects, amountOfPhysicalObjects);

        renderObjects(renderer, objects, amountOfObjects);

        frameCounter++;

        do
        {
            clock_gettime(CLOCK_MONOTONIC, &t2);
            //  while we have not passed 16ms in one frame wait till that has happened
        } while (((t2.tv_sec < t1.tv_sec) | ((t2.tv_sec == t1.tv_sec) & (t2.tv_nsec < t1.tv_nsec))));
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