#if 0
#!/bin/sh
gcc -Wall `sdl-config --cflags` udps.c -o udps `sdl-config --libs` -lSDL_net
 
exit
#endif
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
 
#include "SDL2/SDL_net.h"
 
int main(int argc, char **argv)
{
	UDPsocket sd;       /* Socket descriptor */
	UDPpacket *pRecive;       /* Pointer to packet memory */
	UDPpacket *pSent;

    Uint32 IPclient1=0; 
    Uint32 IPclient2=0;
    Uint32 portClient1=0; 
    Uint32 portClient2=0;
	IPaddress players[4] = {{0,0}};
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
	if (!((pSent = SDLNet_AllocPacket(250))&&(pRecive = SDLNet_AllocPacket(250))))
	{
		fprintf(stderr, "SDLNet_AllocPacket: %s\n", SDLNet_GetError());
		exit(EXIT_FAILURE);
	}
	

	/* Main loop */
	quit = 0;
	while (!quit)
	{
		/* Wait a packet. UDP_Recv returns != 0 if a packet is coming */
		if (SDLNet_UDP_Recv(sd, pRecive))
		{
			int newPlayer = 1;
			int ifNotNewWhichIndex = 0;
			for (int i = 0; i < amountOfPlayers; i++)
			{
				if(players[i].host == pRecive->address.host && players[i].port == pRecive->address.port) 
				{
					newPlayer = 0;
					ifNotNewWhichIndex = i;
				}
			}

			if(newPlayer) {
				if(amountOfPlayers < 4) 
				{
					printf("Connecting player...");
					printf(" assigned number: %d\n", amountOfPlayers);
					players[amountOfPlayers].host = pRecive->address.host;
					players[amountOfPlayers].port = pRecive->address.port;
					sprintf((char *)pSent->data, "%d\n", amountOfPlayers);
					amountOfPlayers++;
					pSent->address.host = pRecive->address.host;
					pSent->address.port = pRecive->address.port;
					pSent->len = strlen((char *)pSent->data) + 1;
					SDLNet_UDP_Send(sd, -1, pSent);
				}
				else 
				{
					printf("Lobby is full couldn't connect new player\n");
				}		
			}

			if(!newPlayer) 
			{
				sscanf((char * )pRecive->data, "%f %f %d\n", &a, &b, &flip);
				sprintf((char *)pSent->data, "%f %f %d\n", a, b, flip);
				pSent->len = strlen((char *)pSent->data) + 1;
				printf("%f %f %d\n", a, b, flip);
				for (int i = 0; i < amountOfPlayers; i++)
				{
					if(amountOfPlayers != ifNotNewWhichIndex) 
					{
						pSent->address.host = players[i].host;	/* Set the destination host */
		    			pSent->address.port = players[i].port;
            			SDLNet_UDP_Send(sd, -1, pSent);	
					}	
				}			
			}	


            // if(IPclient1 == 0 && portClient1 == 0){
            //     printf("Client 1\n");
            //     IPclient1 = pRecive->address.host;
            //     portClient1 = pRecive->address.port;
            // }else if(pRecive->address.port != portClient1  && IPclient2 == 0){
            //     printf("Client 2\n");
            //     IPclient2 = pRecive->address.host;
            //     portClient2 = pRecive->address.port;
            // }else{
            //     if (pRecive->address.port == portClient1){
            //         printf("Recived data\n");
            //         if(IPclient2 != 0){
            //             printf("Send to Client 2\n");
            //             pSent->address.host = IPclient2;	/* Set the destination host */
		    //             pSent->address.port = portClient2;
            //             sscanf((char * )pRecive->data, "%f %f %d\n", &a, &b, &flip);
            //             printf("%f %f %d\n", a, b, flip);
            //             sprintf((char *)pSent->data, "%f %f %d\n", a, b, flip);
            //             pSent->len = strlen((char *)pSent->data) + 1;
            //             SDLNet_UDP_Send(sd, -1, pSent);
            //         }
            //     } else if (pRecive->address.port == portClient2){
                    
            //     }
                
            // }
            

			/* Quit if packet contains "quit" */
			if (strcmp((char *)pSent->data, "quit") == 0)
				quit = 1;
		}		
	}

	/* Clean and exit */
	SDLNet_FreePacket(pSent);
    SDLNet_FreePacket(pRecive);
	SDLNet_Quit();
	return EXIT_SUCCESS;
} 
