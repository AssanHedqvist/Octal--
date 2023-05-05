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
 
int main(int argc, char **argv)
{
	
	UDPsocket sd;       /* Socket descriptor */
	UDPpacket *pReceive;       /* Pointer to packet memory */
	UDPpacket *pSent;
	
	unsigned char takenPlayerSlots[4] = {0};
	IPaddress playersIP[4] = {{0,0}};
	Player playersObject[4] = {{0, 0, 0, 0, 0, 0, 0}};
	KeyboardStates playerInputs[4] = {{{0}}};
	unsigned char playersHP[4] = {0};
	unsigned char playerLives[4] = {4};
	int amountOfPlayers = 0;
	
    int quit = 0;
	
    PhysicsObject physicsObjects[5] = {{{0.f, 0.f}, {0.f, 0.f}, {0.f, 0.f}, {0.f, 0.f}, 0}};
	int amountOfPhysicalObjects = 5;
	
	//  (platform)
    physicsObjects[0].acceleration = vec2(0.f, 0.f);
    physicsObjects[0].pos = vec2(100, 150);
    physicsObjects[0].oldPos = physicsObjects[0].pos;
    physicsObjects[0].extents = vec2(600, 150);
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

	clock_gettime(CLOCK_MONOTONIC, &t1);

	/* Main loop */
	
	while (!quit)
	{
		clock_gettime(CLOCK_MONOTONIC, &t2);

		if(amountOfPlayers > 0) {
			getTimeDifference(&t1, &t2, &timeToDoPhysics);
			
			executePhysicsAmount = timeToDoPhysics.tv_nsec / 16666666;
			timeToDoPhysics.tv_nsec %= 16666666;

			//printf("Time received: %llu,%09ld\n", timeToDoPhysics.tv_sec, timeToDoPhysics.tv_nsec);
		}
		
		t1 = t2;

		/* Wait a packet. UDP_Recv returns != 0 if a packet is coming */
		while (SDLNet_UDP_Recv(sd, pReceive) == 1)
		{
			int newPlayer = 1;
			int ifNotNewWhichIndex = 0;
			for (int i = 0; i < 4; i++)
			{
				if(playersIP[i].host == pReceive->address.host && playersIP[i].port == pReceive->address.port) 
				{
					newPlayer = 0;
					ifNotNewWhichIndex = i;
					break;
				}
			}

			if(newPlayer) {
				int ifNewWhichIndex = 0;
				if(amountOfPlayers < 4) 
				{
					for (int i = 0; i < 4; i++)
					{	
						if(takenPlayerSlots[i] == 0) 
						{
							ifNewWhichIndex = i;
							break;
						}
					}

					memcpy(pSent->data, (void*)&ifNewWhichIndex, 4);

					playersIP[ifNewWhichIndex].host = pReceive->address.host;
					pSent->address.host = pReceive->address.host;
					playersIP[ifNewWhichIndex].port = pReceive->address.port;
					pSent->address.port = pReceive->address.port;
					
					pSent->len = 4;
					
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

			if(!newPlayer) 
			{
				memcpy((void*)&playerInputs[ifNotNewWhichIndex].keyState,pReceive->data,32);

				handlePlayerInputs(&playersObject[ifNotNewWhichIndex], (1.f/240.0f), &playerInputs[ifNotNewWhichIndex]);

				unsigned char disconnecting = 0;
				memcpy((void*)&disconnecting,pReceive->data+32,1);
				if(disconnecting) 
				{			
					amountOfPlayers--;
					playersIP[ifNotNewWhichIndex].host = 0;
					playersIP[ifNotNewWhichIndex].port = 0;
					takenPlayerSlots[ifNotNewWhichIndex] = 0;
					printf("Disconnecting Player: %d\n", ifNotNewWhichIndex);
				}
			}	
		}	
		
		while (executePhysicsAmount > 0)
		{
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
				memcpy(pSent->data, (void*)&physicsObjects, 180);
				pSent->len = 180;

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

