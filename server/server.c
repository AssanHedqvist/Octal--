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

void doWeNeedToSimulatePhysics(struct timespec* before, struct timespec* after, struct timespec* remainingTime) {
	struct timespec immediateTime;
	if(after->tv_sec >= before->tv_sec) {
		immediateTime.tv_sec = after->tv_sec - before->tv_sec;
		if(immediateTime.tv_sec > 0) 
		{
			immediateTime.tv_nsec = (1000000000 + after->tv_nsec) - before->tv_nsec;
			immediateTime.tv_sec = immediateTime.tv_sec > 1 ? immediateTime.tv_sec - 1 : immediateTime.tv_sec;
		}
		else if(immediateTime.tv_sec == 0) 
		{
			immediateTime.tv_nsec = after->tv_nsec - before->tv_nsec;
		}

		remainingTime->tv_sec += immediateTime.tv_sec + ((remainingTime->tv_nsec + immediateTime.tv_nsec) / 1000000000);
        remainingTime->tv_nsec = ((remainingTime->tv_nsec + immediateTime.tv_nsec) % 1000000000);
	}
}

double getTime(struct timespec after, struct timespec before) {
    return ((double)after.tv_sec + ((double)after.tv_nsec * 1e-9)) -
           ((double)before.tv_sec + ((double)before.tv_nsec * 1e-9));
}
 
int main(int argc, char **argv)
{
	
	UDPsocket sd;       /* Socket descriptor */
	UDPpacket *pReceive;       /* Pointer to packet memory */
	UDPpacket *pSent;

    Uint32 IPclient1=0; 
    Uint32 IPclient2=0;
    Uint32 portClient1=0; 
    Uint32 portClient2=0;
	IPaddress playersIP[4] = {{0,0}};
	char takenPlayerSlots[4] = {0};
	int amountOfPlayers = 0;

	Player playersObject[4] = {{0, 0, 0, 0, 0, 0, 0}};
	
    PhysicsObject physicsObjects[5] = {{{0.f, 0.f}, {0.f, 0.f}, {0.f, 0.f}, {0.f, 0.f}, 0, 0}};
	int amountOfPhysicalObjects = 5;

    

    int quit;
 
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
 
	/* Make space for the packet */
	if (!((pSent = SDLNet_AllocPacket(250))&&(pReceive = SDLNet_AllocPacket(250))))
	{
		fprintf(stderr, "SDLNet_AllocPacket: %s\n", SDLNet_GetError());
		exit(EXIT_FAILURE);
	}
	
	playersObject[0].physics = &physicsObjects[1];
    playersObject[1].physics = &physicsObjects[2];
 	playersObject[2].physics = &physicsObjects[3];
    playersObject[3].physics = &physicsObjects[4];

	KeyboardStates states[4] = {{0}};
	
	struct timespec t1, t2, timeLeft, timeCheck;

	int executePhysicsAmountOfTimes = 0;
	quit = 0;

	clock_gettime(CLOCK_MONOTONIC, &t1);

	double timer;
	
	/* Main loop */
	
	while (!quit)
	{
		clock_gettime(CLOCK_MONOTONIC, &t2);

		if(amountOfPlayers > 0) {
			timer += getTime(t2, t1);
			//printf("Timer: %lf\n", timer);
		}
		t1 = t2;

		/* Wait a packet. UDP_Recv returns != 0 if a packet is coming */
		if (SDLNet_UDP_Recv(sd, pReceive))
		{
			int newPlayer = 1;
			int ifNewWhichIndex = 0;
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
					int tmp = 0;
					printf("Player connected assigned number: %d\n", ifNewWhichIndex);
					takenPlayerSlots[ifNewWhichIndex] = 1;

					playersIP[ifNewWhichIndex].host = pReceive->address.host;
					playersIP[ifNewWhichIndex].port = pReceive->address.port;
					memmove(pSent->data, (void*)&ifNewWhichIndex, 4);
					pSent->len = 4;
					pSent->address.host = pReceive->address.host;
					pSent->address.port = pReceive->address.port;

					amountOfPlayers++;
					
					SDLNet_UDP_Send(sd, -1, pSent);
				}
				else 
				{
					printf("Lobby is full couldn't connect new player\n");
				}		
			}

			if(!newPlayer) 
			{
				memmove((void*)&states[ifNotNewWhichIndex].keyState,pReceive->data,32);

				handlePlayerInputs(&playersObject[ifNotNewWhichIndex], (1.f/240.0f), &states[ifNotNewWhichIndex]);

				unsigned char disconnecting = 0;
				memmove((void*)&disconnecting,pReceive->data+32,1);
				if(disconnecting) 
				{
					printf("Disconnecting Player: %d\n", ifNotNewWhichIndex);
					amountOfPlayers--;
					playersIP[ifNotNewWhichIndex].host = 0;
					playersIP[ifNotNewWhichIndex].port = 0;
					takenPlayerSlots[ifNotNewWhichIndex] = 0;
				}
			}	
		}	

		if(timer >= 0.0166666666666666666666666666666)
		{
			for (int i = 0; i < amountOfPhysicalObjects; i++)
            {
                physicsObjects[i].recentCollision = 0;
            }

			for (int i = 0; i < 4; i++)
        	{
        	    boundarySolve(playersObject);
        	    constraintSolve(physicsObjects, amountOfPhysicalObjects);
        	    updatePositions(physicsObjects, amountOfPhysicalObjects, (1.f/240.0f));
        	}

			memmove(pSent->data, (void*)&physicsObjects, 180);
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

			//clock_gettime(CLOCK_MONOTONIC, &timeCheck);
			//printf("Time sent: %d,%09d\n", timeCheck.tv_sec, timeCheck.tv_nsec);
			timer -= 0.0166666666666666666666666666666;
		}
		

	}

	/* Clean and exit */
	SDLNet_FreePacket(pSent);
    SDLNet_FreePacket(pReceive);
	SDLNet_Quit();
	return EXIT_SUCCESS;
} 

