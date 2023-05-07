
INCLUDE = C:\msys64\mingw64\include
LDFLAGS = -lmingw32 -lSDL2main -lSDL2 -lSDL2_net 

LIBS = C:\msys64\mingw64\lib

host: server.o vec2.o physicsObject.o renderObject.o keyboard.o player.o attacks.o
	gcc -o host.exe -O2 server.o vec2.o physicsObject.o renderObject.o keyboard.o player.o attacks.o -L$(LIBS) $(LDFLAGS) -Wall
server.o:
	gcc -c server.c -I$(INCLUDE) -Wall
vec2.o: 
	gcc -c ../src/vec2.c -Wall
keyboard.o:
	gcc -c ../src/keyboard.c -I$(INCLUDE) -Wall
physicsObject.o: vec2.o 
	gcc -c ../src/physicsObject.c -Wall
renderObject.o:
	gcc -c ../src/renderObject.c -I$(INCLUDE) -Wall
player.o: keyboard.o renderObject.o physicsObject.o 
	gcc -c ../src/player.c -Wall
attacks.o: keyboard.o player.o
	gcc -c ../src/attacks.c -I$(INCLUDE) -Wall
clean:
	rm *.o
	rm *.exe