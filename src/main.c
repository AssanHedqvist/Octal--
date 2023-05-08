#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_net.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include "../include/vec2.h"
#include "../include/physicsObject.h"
#include "../include/renderObject.h"
#include "../include/keyboard.h"
#include "../include/player.h"
#include "../include/text.h"
#include "../include/attacks.h"
#include "../include/sounds.h"
#include "../include/menu.h"
#include "../include/mouse.h"

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

#define BUTTON_X 140
#define BUTTON_Y 210
#define BUTTON_WIDTH 120
#define BUTTON_HEIGHT 40
#define BUTTON_GAP 60

typedef enum 
{
    MENU,
    RUNNING,
    CLOSED
} GameStates;

typedef enum {
    JOIN_REQUEST = 0,
    CLIENT_KEYBOARD = 1,
	DISCONNECTING = 2
} clientMessage;

typedef enum  {
    JOIN_ANSWER = 0,
    PHYSICS_INFO = 1
} serverMessage;

int main(int argv, char **args)
{
    UDPsocket sd;
    IPaddress serverAddress;
    UDPpacket *toServer;
    UDPpacket *fromServer;
    Player players[4] = {{0, 0, 0, 0, 0, 0, 0, 0, 0, 0}};
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
    if (SDLNet_ResolveHost(&serverAddress, "130.229.128.1", 31929) == -1)
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

    unsigned char thisComputersPlayerIndex = 0;

    //  server connecting code

    unsigned char messageType;
    while(1) 
    {
        toServer->address.host = serverAddress.host;
        toServer->address.port = serverAddress.port;

        messageType = JOIN_REQUEST;

        memcpy(toServer->data, (void *)&messageType, 1);

        toServer->len = 1;

        SDLNet_UDP_Send(sd, -1, toServer);

        if(SDLNet_UDP_Recv(sd, fromServer)==1) 
        {
            if(fromServer->data[0] == JOIN_ANSWER) 
            {
                memcpy((void *)&thisComputersPlayerIndex, fromServer->data+1, 1);
                break;
            }
        }
    }

    printf("Client: %d\n", thisComputersPlayerIndex);

    //  server connecting code

    SDL_Window *window = SDL_CreateWindow("Hello Octal--!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, 0);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);
    TTF_Font *font = TTF_OpenFont("./resources/fonts/moiser.ttf", 100);

    SDL_Event event;

    // initialize SDL_mixer
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
    Mix_Music *backgroundMusic = Mix_LoadMUS("resources/music/tempMusicWhatIsLove.mp3");

    SoundEffect soundEffect;
    loadSoundEffects(&soundEffect);

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
    renderObjects[1].imageExtents = (SDL_Rect){0, 0, 1000, 75};
    renderObjects[1].screenExtents = (SDL_Rect){100, 300, 600, 53};
    renderObjects[1].flip = 0;

    renderObjects[2].order = 2;
    renderObjects[2].texture = IMG_LoadTexture(renderer, "resources/gridnetForSprites.png");
    renderObjects[2].imageExtents = (SDL_Rect){0, 0, 128, 256};
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
    physicsObjects[0].pos = vec2(100, 247);
    physicsObjects[0].oldPos = physicsObjects[0].pos;
    physicsObjects[0].extents = vec2(600, 53);
    physicsObjects[0].flags = 0;

    //  (player 0)
    physicsObjects[1].acceleration = vec2(0.f, -982.0f);
    physicsObjects[1].pos = vec2(150, 536);
    physicsObjects[1].oldPos = physicsObjects[1].pos;
    physicsObjects[1].extents = vec2(32, 64);
    physicsObjects[1].flags = (DYNAMIC | PLAYER);
    //  (player 1)
    physicsObjects[2].acceleration = vec2(0.f, -982.0f);
    physicsObjects[2].pos = vec2(316.6666666666666666666666666666f, 536);
    physicsObjects[2].oldPos = physicsObjects[2].pos;
    physicsObjects[2].extents = vec2(32, 64);
    physicsObjects[2].flags = (DYNAMIC | PLAYER);

    //  (player 2)
    physicsObjects[3].acceleration = vec2(0.f, -982.0f);
    physicsObjects[3].pos = vec2(483.33333333333333333333333333334f, 536);
    physicsObjects[3].oldPos = physicsObjects[3].pos;
    physicsObjects[3].extents = vec2(32, 64);
    physicsObjects[3].flags = (DYNAMIC | PLAYER);

    //  (player 3)
    physicsObjects[4].acceleration = vec2(0.f, -982.0f);
    physicsObjects[4].pos = vec2(650, 536);
    physicsObjects[4].oldPos = physicsObjects[4].pos;
    physicsObjects[4].extents = vec2(32, 64);
    physicsObjects[4].flags = (DYNAMIC | PLAYER);

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

    SDL_Texture *backgroundTexture = IMG_LoadTexture(renderer, "resources/menu/menu.png");
    SDL_Rect backgroundRect = {0, 0, 800, 600};
    MenuButton buttons[5];
    createButtons(renderer, buttons, font);

    KeyboardStates keyboardInputs = {{0}};
    MouseState mouseInputs = {0, 0, 0};

    struct timespec t1, t2;

    int currentGameState = MENU;
    int inGameMenuOpen = 0;
    int wentIntoMenu = 0;

    while (currentGameState != CLOSED)
    {
        clock_gettime(CLOCK_MONOTONIC, &t1);
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_QUIT:

                currentGameState = CLOSED;
                messageType = DISCONNECTING;

                break;
            case SDL_KEYDOWN:
            case SDL_KEYUP:
                handleKeyboardInputs(&keyboardInputs, event.key.keysym.scancode, event.type);
                break;
            case SDL_MOUSEMOTION:
                updateMousePos(&mouseInputs);
                break;
            case SDL_MOUSEBUTTONDOWN:
            case SDL_MOUSEBUTTONUP:
                handleMouseInputs(&mouseInputs, event.button.button, event.type);
                break;
            }
        }
        switch (currentGameState)
        {
        case MENU:
            if (isMouseButtonPressed(&mouseInputs, SDL_BUTTON_LEFT))
            {
                for (int i = 0; i < 3; i++)
                {
                    if (mouseInputs.x >= buttons[i].rect.x &&
                        mouseInputs.x <= buttons[i].rect.x + buttons[i].rect.w &&
                        mouseInputs.y >= buttons[i].rect.y &&
                        mouseInputs.y <= buttons[i].rect.y + buttons[i].rect.h)
                    {
                        switch (i)
                        {
                        case 0:
                            currentGameState = RUNNING;
                            Mix_PlayChannel(-1, soundEffect.buttonClick,0);
                            // Mix_PlayMusic(backgroundMusic,-1); // music plays when game starts
                            break;
                        case 1:
                            Mix_PlayChannel(-1, soundEffect.buttonClick,0);
                            break;
                        case 2:
                            Mix_PlayChannel(-1, soundEffect.buttonClick,0);
                            currentGameState = CLOSED;
                            messageType = DISCONNECTING;
                            break;
                        default:
                            break;
                        }
                    }
                }
            }

            SDL_RenderClear(renderer);
            renderMenu(renderer, backgroundTexture, backgroundRect, buttons);
            SDL_RenderPresent(renderer);
            break;
        case RUNNING:
            messageType = CLIENT_KEYBOARD;
            memcpy(toServer->data, (void *)&messageType, 1);
            memcpy(toServer->data+1, (void *)&keyboardInputs.keyState, 32);
           
            toServer->len = 33;

            SDLNet_UDP_Send(sd, -1, toServer);

            if (isMouseButtonPressed(&mouseInputs, SDL_BUTTON_LEFT) && inGameMenuOpen)
            {

                for (int i = 3; i < 5; i++)
                {
                    if (mouseInputs.x >= buttons[i].rect.x &&
                        mouseInputs.x <= buttons[i].rect.x + buttons[i].rect.w &&
                        mouseInputs.y >= buttons[i].rect.y &&
                        mouseInputs.y <= buttons[i].rect.y + buttons[i].rect.h)
                    {
                        switch (i)
                        {
                        case 3:
                            inGameMenuOpen = 0;
                            Mix_PlayChannel(-1, soundEffect.buttonClick,0);
                            break;
                        case 4:
                            inGameMenuOpen = 0;
                            Mix_PlayChannel(-1, soundEffect.buttonClick,0);
                            currentGameState = MENU;
                        default:
                            break;
                        }
                    }
                }
            }

            if (isKeyDown(&keyboardInputs, SDL_SCANCODE_P))
            {
                // togglePlay();                           //press p to toggle music
            }

            if (isKeyDown(&keyboardInputs, SDL_SCANCODE_ESCAPE) && wentIntoMenu == 0)
            {
                inGameMenuOpen = !inGameMenuOpen;
                wentIntoMenu = 1;
            }

            if (!isKeyDown(&keyboardInputs, SDL_SCANCODE_ESCAPE) && wentIntoMenu == 1)
            {
                wentIntoMenu = 0;
            }

            if (isKeyDown(&keyboardInputs, SDL_SCANCODE_E))
            {
                players[0].health += 10;
            }

          
            if (!inGameMenuOpen)
            {
                handlePlayerInputs(&players[thisComputersPlayerIndex], DT, &keyboardInputs/*, soundEffect*/);
            }

            for (int i = 0; i < amountOfPhysicalObjects; i++)
            {
                physicsObjects[i].flags &= 0b00001111;
            }

            for (int i = 0; i < SUB_STEPS; i++)
            {
                boundarySolve(physicsObjects, amountOfPhysicalObjects);
                constraintSolve(physicsObjects, amountOfPhysicalObjects);
                updatePositions(physicsObjects, amountOfPhysicalObjects, DT);
            }

            while (SDLNet_UDP_Recv(sd, fromServer) == 1)
            {
                if(fromServer->data[0] == PHYSICS_INFO) 
                {
                    memcpy((void*)&physicsObjects, fromServer->data+1, 180); 

                    memcpy((void*)&players[0].health,fromServer->data+181, 2);
                    memcpy((void*)&players[1].health,fromServer->data+183, 2);
                    memcpy((void*)&players[2].health,fromServer->data+185, 2);
                    memcpy((void*)&players[3].health,fromServer->data+187, 2);

                    memcpy((void*)&players[0].lives,fromServer->data+189, 1);
                    memcpy((void*)&players[1].lives,fromServer->data+190, 1);
                    memcpy((void*)&players[2].lives,fromServer->data+191, 1);
                    memcpy((void*)&players[3].lives,fromServer->data+192, 1);

                    memcpy((void*)&players[0].animationState,fromServer->data+193, 1);
                    memcpy((void*)&players[1].animationState,fromServer->data+194, 1);
                    memcpy((void*)&players[2].animationState,fromServer->data+195, 1);
                    memcpy((void*)&players[3].animationState,fromServer->data+196, 1);

                    memcpy((void*)&players[0].render->flip,fromServer->data+197, 1);
                    memcpy((void*)&players[1].render->flip,fromServer->data+198, 1);
                    memcpy((void*)&players[2].render->flip,fromServer->data+199, 1);
                    memcpy((void*)&players[3].render->flip,fromServer->data+200, 1);
                }       
            }

            handlePlayerAnimation(players);

            updateRenderWithPhysics(renderObjects, physicsObjects, amountOfPhysicalObjects);

            SDL_RenderClear(renderer);
            render(renderer, renderObjects, amountOfRenderObjects);
            renderPlayerHealth(players, 4, renderer, font, 100, 550);
            if (inGameMenuOpen)
            {
                renderIngameMenu(renderer, backgroundRect, buttons);
            }

            SDL_RenderPresent(renderer);
            break;
        }
        frameCounter++;

        //  16638935 = (1/60.1) * 1000000000
        t1.tv_sec += ((t1.tv_nsec + 16638935) / 1000000000);
        t1.tv_nsec = ((t1.tv_nsec + 16638935) % 1000000000);
        do
        {
            clock_gettime(CLOCK_MONOTONIC, &t2);
            //  while we have not passed 16ms in one frame wait till that has happened
        } while (((t2.tv_sec < t1.tv_sec) || ((t2.tv_sec == t1.tv_sec) && (t2.tv_nsec < t1.tv_nsec))));
    }

    messageType = DISCONNECTING;
    memcpy(toServer->data, (void *)&messageType, 1);
    memcpy(toServer->data+1, (void *)&keyboardInputs.keyState, 32);
   
    toServer->len = 33;

    SDLNet_UDP_Send(sd, -1, toServer);

    SDLNet_Quit();
    SDLNet_FreePacket(toServer);
    SDLNet_FreePacket(fromServer);

    TTF_CloseFont(font);

    TTF_Quit();

    Mix_FreeMusic(backgroundMusic);
    freeSoundEffects(&soundEffect);
    Mix_CloseAudio();

    for (int i = 0; i < amountOfRenderObjects; i++)
    {
        SDL_DestroyTexture(renderObjects[i].texture);
    }

    freeButtons(buttons, 5);

    SDL_DestroyTexture(backgroundTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
