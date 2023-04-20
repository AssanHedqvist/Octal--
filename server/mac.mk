serverOchClient: udps.c udpc.c
	gcc -o server udpsMirror.c -lSDL2_net
	gcc -o client udpc.c -lSDL2_net