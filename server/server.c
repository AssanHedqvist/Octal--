#if 0
#!/bin/sh
gcc -Wall `sdl-config --cflags` udps.c -o udps `sdl-config --libs` -lSDL_net
 
exit
#endif
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "SDL2/SDL.h"
#include "SDL2/SDL_net.h"
#include "../include/keyboard.h"
#include "../include/physicsObject.h"
#include "../include/player.h"
#include "../include/renderObject.h"
#include "../include/attacks.h"

void getTimeDifference(struct timespec* before, struct timespec* after, struct timespec* remainingTime) {
	struct timespec tmpTime;
	if(after->tv_sec >= before->tv_sec) {
		tmpTime.tv_sec = after->tv_sec - before->tv_sec;
		if(tmpTime.tv_sec > 0) 
		{
			tmpTime.tv_nsec = (1000000000 + after->tv_nsec) - before->tv_nsec;
			tmpTime.tv_sec = tmpTime.tv_sec > 1llu ? tmpTime.tv_sec - 1llu : tmpTime.tv_sec;
		}
		else if(tmpTime.tv_sec == 0) 
		{
			tmpTime.tv_nsec = after->tv_nsec - before->tv_nsec;
		}

		remainingTime->tv_sec +=  ((remainingTime->tv_nsec - tmpTime.tv_sec * 1000000000 + tmpTime.tv_nsec) / 1000000000);
        remainingTime->tv_nsec = ((remainingTime->tv_nsec + tmpTime.tv_nsec) % 1000000000);
	}
}

typedef enum {
    JOIN_REQUEST = 0,
    CLIENT_KEYBOARD = 1,
	DISCONNECTING = 2
} clientMessage;

typedef enum {
    JOIN_ANSWER = 0,
    PHYSICS_INFO = 1
} serverMessage;

char checkWinner(int *amountOfPlayers, Player players[4], unsigned char takenPlayerSlots[4])
{
	int nAlive = 0, nWinner = -1;
	for (int i = 0; i < 4; i++) 
	{
		if (takenPlayerSlots[i] && players[i].lives > 0)
		{
			nAlive++;
			nWinner = i;
		}
	}
		
	if (nAlive > 1)
		return -1;
	return nWinner;	
}
 
int main(int argc, char **argv)
{
	
	UDPsocket sd;       /* Socket descriptor */
	UDPpacket *pReceive;       /* Pointer to packet memory */
	UDPpacket *pSent;
	
	unsigned char takenPlayerSlots[4] = {0};
	IPaddress playersIP[4] = {{0,0}};
	Player playersObject[4] = {{0, 0, 0, 0, 0, 0, 0, 0, 0, 0}};
	KeyboardStates playerInputs[4] = {{{0}}};
	unsigned char playerFlip[4] = {0};
	int amountOfPlayers = 0;

	int soundsToPlay = 0;
	
    int quit = 0;
	
    PhysicsObject physicsObjects[5] = {{{0.f, 0.f}, {0.f, 0.f}, {0.f, 0.f}, {0.f, 0.f}, 0}};
	int amountOfPhysicalObjects = 5;
	
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
    physicsObjects[1].flags = DYNAMIC | PLAYER;
    //  (player 1)
    physicsObjects[2].acceleration = vec2(0.f, -982.0f);
    physicsObjects[2].pos = vec2(316.6666666666666666666666666666f, 536);
    physicsObjects[2].oldPos = physicsObjects[2].pos;
    physicsObjects[2].extents = vec2(32, 64);
    physicsObjects[2].flags = DYNAMIC | PLAYER;

    //  (player 2)
    physicsObjects[3].acceleration = vec2(0.f, -982.0f);
    physicsObjects[3].pos = vec2(483.33333333333333333333333333334f, 536);
    physicsObjects[3].oldPos = physicsObjects[3].pos;
    physicsObjects[3].extents = vec2(32, 64);
    physicsObjects[3].flags = DYNAMIC | PLAYER;

    //  (player 3)
    physicsObjects[4].acceleration = vec2(0.f, -982.0f);
    physicsObjects[4].pos = vec2(650, 536);
    physicsObjects[4].oldPos = physicsObjects[4].pos;
    physicsObjects[4].extents = vec2(32, 64);
    physicsObjects[4].flags = DYNAMIC | PLAYER;
 
	/* Initialize SDL_net */
	if (SDLNet_Init() < 0)
	{
		fprintf(stderr, "SDLNet_Init: %s\n", SDLNet_GetError());
		exit(EXIT_FAILURE);
	}

	/* Open a socket */
	if (!(sd = SDLNet_UDP_Open(31929)))
	{
		fprintf(stderr, "SDLNet_UDP_Open: %s\n", SDLNet_GetError());
		exit(EXIT_FAILURE);
	}

	/* Make space for the packet */
	if (!((pSent = SDLNet_AllocPacket(250)) && (pReceive = SDLNet_AllocPacket(250))))
	{
		fprintf(stderr, "SDLNet_AllocPacket: %s\n", SDLNet_GetError());
		exit(EXIT_FAILURE);
	}
	
	playersObject[0].physics = &physicsObjects[1];
    playersObject[1].physics = &physicsObjects[2];
 	playersObject[2].physics = &physicsObjects[3];
    playersObject[3].physics = &physicsObjects[4];
	struct timespec t1, t2, timeToDoPhysics = {0,0};

	int executePhysicsAmount = 0;

	unsigned char messageType = 0;

	clock_gettime(CLOCK_MONOTONIC, &t1);

	/* Main loop */
	
	while (!quit)
	{
		clock_gettime(CLOCK_MONOTONIC, &t2);

		if(amountOfPlayers > 0) {
			getTimeDifference(&t1, &t2, &timeToDoPhysics);
			
			executePhysicsAmount = timeToDoPhysics.tv_nsec / 16666666;
			timeToDoPhysics.tv_nsec %= 16666666;
		}
		
		t1 = t2;

		/* Wait a packet. UDP_Recv returns != 0 if a packet is coming */
		while (SDLNet_UDP_Recv(sd, pReceive) == 1)
		{
			int newPlayer = 1;
			unsigned char ifNotNewWhichIndex = 0;
			
			for (int i = 0; i < 4; i++)
			{
				if(playersIP[i].host == pReceive->address.host && playersIP[i].port == pReceive->address.port) 
				{
					newPlayer = 0;
					ifNotNewWhichIndex = i;
					break;
				}
			}

			switch (pReceive->data[0])
			{
				case JOIN_REQUEST:
					if(newPlayer) {		
						if(amountOfPlayers < 4) 
						{
							unsigned char ifNewWhichIndex = 0;
							for (int i = 0; i < 4; i++)
							{	
								if(takenPlayerSlots[i] == 0) 
								{
									ifNewWhichIndex = i;
									break;
								}
							}

							messageType = JOIN_ANSWER;
							memcpy(pSent->data, (void*)&messageType, 1);
							memcpy(pSent->data+1, (void*)&ifNewWhichIndex, 1);

							playersIP[ifNewWhichIndex].host = pReceive->address.host;
							pSent->address.host = pReceive->address.host;
							playersIP[ifNewWhichIndex].port = pReceive->address.port;
							pSent->address.port = pReceive->address.port;

							pSent->len = 2;

							SDLNet_UDP_Send(sd, -1, pSent);

							takenPlayerSlots[ifNewWhichIndex] = 1;

							printf("Player connected assigned number: %d\n", ifNewWhichIndex);

							amountOfPlayers++;
						}
						else 
						{
							printf("Lobby is full couldn't connect new player\n");
						}		
					}
					else {
						messageType = JOIN_ANSWER;
						memcpy(pSent->data, (void*)&messageType, 1);
						memcpy(pSent->data+1, (void*)&ifNotNewWhichIndex, 1);
						pSent->address.host = playersIP[ifNotNewWhichIndex].host;
						pSent->address.port = playersIP[ifNotNewWhichIndex].port;
						
						pSent->len = 2;

						SDLNet_UDP_Send(sd, -1, pSent);
					}
					break;
				case CLIENT_KEYBOARD:
					if(!newPlayer) 
					{
						memcpy((void*)&playerInputs[ifNotNewWhichIndex].keyState,pReceive->data+1,32);
					}
					break;
				case DISCONNECTING:
					if(!newPlayer) 
					{
						amountOfPlayers--;
						playersIP[ifNotNewWhichIndex].host = 0;
						playersIP[ifNotNewWhichIndex].port = 0;
						takenPlayerSlots[ifNotNewWhichIndex] = 0;
						printf("Disconnecting Player: %d\n", ifNotNewWhichIndex);
					}
					break;
				default:
					break;
			}
		}	
		
		while (executePhysicsAmount > 0)
		{
		
			for (int i = 0; i < 4; i++)
			{
				handlePlayerInputs(&playersObject[i], (1.f/240.0f), &playerInputs[i]);

				//	temporary solution for flip 
				if (isKeyDown(&playerInputs[i], SDL_SCANCODE_A)) 
				{
					playerFlip[i] = 1;
				}

				if (isKeyDown(&playerInputs[i], SDL_SCANCODE_D)) 
				{
					playerFlip[i] = 0;
				}

				if (isKeyDown(&playerInputs[i], SDL_SCANCODE_J) && playersObject[i].timeSinceLastPunch >=  0.13333333333333333333333333333333f) 
				{
					playersObject[i].animationState = PUNCH_0;
				}
				
			}

			lightPunchServer(playersObject, playerFlip, playerInputs);
			handlePlayerAnimationServerAlt(playersObject);
			handlePlayerLives(playersObject);
			
			for (int i = 0; i < amountOfPhysicalObjects; i++)
            {
                physicsObjects[i].flags &= 0b00001111;
            }

			for (int i = 0; i < 4; i++)
        	{
        	    boundarySolve(physicsObjects, amountOfPhysicalObjects);
        	    constraintSolve(physicsObjects, amountOfPhysicalObjects);
        	    updatePositions(physicsObjects, amountOfPhysicalObjects, (1.f/240.0f));
        	}

			if(executePhysicsAmount == 1) 
			{
				if (amountOfPlayers > 1)
				{
					char nWinner = checkWinner(&amountOfPlayers, playersObject, takenPlayerSlots);
					if (nWinner) 
					{
						memcpy(pSent->data+201, (void*)&nWinner, 1);	
					}
				}	

				messageType = PHYSICS_INFO;
				memcpy(pSent->data,  (void*)&messageType, 1);
				memcpy(pSent->data+1, (void*)&physicsObjects, 180);

				memcpy(pSent->data+181, (void*)&playersObject[0].health, 2);
				memcpy(pSent->data+183, (void*)&playersObject[1].health, 2);
				memcpy(pSent->data+185, (void*)&playersObject[2].health, 2);
				memcpy(pSent->data+187, (void*)&playersObject[3].health, 2);

				memcpy(pSent->data+189, (void*)&playersObject[0].lives, 1);
				memcpy(pSent->data+190, (void*)&playersObject[1].lives, 1);
				memcpy(pSent->data+191, (void*)&playersObject[2].lives, 1);
				memcpy(pSent->data+192, (void*)&playersObject[3].lives, 1);

				memcpy(pSent->data+193, (void*)&playersObject[0].animationState, 1);
				memcpy(pSent->data+194, (void*)&playersObject[1].animationState, 1);
				memcpy(pSent->data+195, (void*)&playersObject[2].animationState, 1);
				memcpy(pSent->data+196, (void*)&playersObject[3].animationState, 1);

				memcpy(pSent->data+197, (void*)&playerFlip, 4);

				pSent->len = 202;

				for (int i = 0; i < 4; i++)
				{
					if(takenPlayerSlots[i] == 1) 
					{
					
						pSent->address.host = playersIP[i].host;
						pSent->address.port = playersIP[i].port;

						SDLNet_UDP_Send(sd, -1, pSent);
					}
				}
			}	
			executePhysicsAmount--;
		}
	}	

	/* Clean and exit */
	SDLNet_FreePacket(pSent);
    SDLNet_FreePacket(pReceive);
	SDLNet_Quit();
	return EXIT_SUCCESS;
}