#if 0
#!/bin/sh
gcc -Wall `sdl-config --cflags` udps.c -o udps `sdl-config --libs` -lSDL_net
 
exit
#endif
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
 
#include "SDL2/SDL_net.h"
#include "../include/keyboard.h"
 
int main(int argc, char **argv)
{
	
	UDPsocket sd;       /* Socket descriptor */
	UDPpacket *pReceive;       /* Pointer to packet memory */
	UDPpacket *pSent;

    Uint32 IPclient1=0; 
    Uint32 IPclient2=0;
    Uint32 portClient1=0; 
    Uint32 portClient2=0;
	IPaddress players[4] = {{0,0}};
	char freePlayerSlots[4] = {0};
	int amountOfPlayers = 0;

    int quit, flip; 
	float a, b;
 
	/* Initialize SDL_net */
	if (SDLNet_Init() < 0)
	{
		fprintf(stderr, "SDLNet_Init: %s\n", SDLNet_GetError());
		exit(EXIT_FAILURE);
	}

	/* Open a socket */
	if (!(sd = SDLNet_UDP_Open(15661)))
	{
		fprintf(stderr, "SDLNet_UDP_Open: %s\n", SDLNet_GetError());
		exit(EXIT_FAILURE);
	}
 
	/* Make space for the packet */
	if (!((pSent = SDLNet_AllocPacket(250))&&(pReceive = SDLNet_AllocPacket(250))))
	{
		fprintf(stderr, "SDLNet_AllocPacket: %s\n", SDLNet_GetError());
		exit(EXIT_FAILURE);
	}
	
	KeyboardStatesAlt states[4];


	
	
	/* Main loop */
	quit = 0;
	while (!quit)
	{
		/* Wait a packet. UDP_Recv returns != 0 if a packet is coming */
		if (SDLNet_UDP_Recv(sd, pReceive))
		{
			int newPlayer = 1;
			int ifNewWhichIndex = 0;
			int ifNotNewWhichIndex = 0;
			for (int i = 0; i < 4; i++)
			{
				if(players[i].host == pReceive->address.host && players[i].port == pReceive->address.port) 
				{
					newPlayer = 0;
					ifNotNewWhichIndex = i;
				}
			}

			if(newPlayer) {
				if(amountOfPlayers < 4) 
				{
					for (int i = 0; i < 4; i++)
					{	
						if(freePlayerSlots[i] == 0) 
						{
							ifNewWhichIndex = i;
							break;
						}
					}
					int tmp = 0;
					printf("Connecting player...");
					printf(" assigned number: %d\n", ifNewWhichIndex);
					freePlayerSlots[ifNewWhichIndex] = 1;

					players[ifNewWhichIndex].host = pReceive->address.host;
					players[ifNewWhichIndex].port = pReceive->address.port;

					memmove(pSent->data, (void*)&amountOfPlayers, 4);
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

				if(isKeyDown(&states[ifNotNewWhichIndex], SDL_SCANCODE_ESCAPE)) 
				{
					printf("Disconnecting Player: %d\n", ifNotNewWhichIndex);
					amountOfPlayers--;
					players[ifNotNewWhichIndex].host = 0;
					players[ifNotNewWhichIndex].port = 0;
					freePlayerSlots[ifNotNewWhichIndex] = 0;
				}
			}	

			/* Quit if packet contains "quit" */
			if (strcmp((char *)pSent->data, "quit") == 0)
				quit = 1;
		}		
	}

	/* Clean and exit */
	SDLNet_FreePacket(pSent);
    SDLNet_FreePacket(pReceive);
	SDLNet_Quit();
	return EXIT_SUCCESS;
} 
