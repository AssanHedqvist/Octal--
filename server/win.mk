
INCLUDE = C:\msys64\mingw64\include
LDFLAGS = -lmingw32 -lSDL2main -lSDL2 -lSDL2_net 

LIBS = C:\msys64\mingw64\lib

host: server.o vec2.o physicsObject.o renderObject.o keyboard.o player.o
	gcc -o host.exe -O2 server.o vec2.o physicsObject.o renderObject.o keyboard.o player.o -L$(LIBS) $(LDFLAGS)
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
	rm *.exe
	rm *.o