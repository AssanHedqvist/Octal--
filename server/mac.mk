#	run this with 
#   "make -f mac.mk"
#	may be runnable for all OS

serverAndClient: server.c client.c
	gcc -o server server.c -lSDL2_net
	gcc -o client client.c -lSDL2_net