#	run this with 
#   "make -f mac.mk"

# Any include directories
INCLUDE = /opt/homebrew/include

# Any flags for the compiler
CFLAGS = -g -I$(INCLUDE) -c

# Any libraries to link with
#	LIBS = /usr/local/lib
LIBS = /opt/homebrew/lib

host: server.o vec2.o physicsObject.o renderObject.o keyboard.o player.o attacks.o
	gcc -o host server.o vec2.o physicsObject.o renderObject.o keyboard.o player.o attacks.o -L$(LIBS) -lSDL2_net -lSDL2main -lSDL2 -lSDL2_mixer
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
attacks.o: keyboard.o player.o
	gcc -c ../src/attacks.c -I$(INCLUDE)

clean:
	rm -f *.o
	rm -f host