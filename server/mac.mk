#	run this with 
#   "make -f mac.mk"

# Any include directories
INCLUDE = /opt/homebrew/include

# Any flags for the compiler
CFLAGS = -g -I$(INCLUDE) -c

# Any libraries to link with
#	LIBS = /usr/local/lib
LIBS = /opt/homebrew/lib

host: server.o vec2.o physicsObject.o renderObject.o keyboard.o player.o
	gcc -o host server.o vec2.o physicsObject.o renderObject.o keyboard.o player.o -L$(LIBS) -lSDL2_net -lSDL2main -lSDL2
server.o:
	gcc -c server.c -I$(INCLUDE)
vec2.o: 
	gcc -c ../src/vec2.c
keyboard.o:
	gcc -c ../src/keyboard.c -I$(INCLUDE)
physicsObject.o: vec2.o 
	gcc -c ../src/physicsObject.c	
renderObject.o:
	gcc -c ../src/renderObject.c -I$(INCLUDE)
player.o: keyboard.o renderObject.o physicsObject.o
	gcc -c ../src/player.c
clean:
	rm -f *.o
	rm -f host