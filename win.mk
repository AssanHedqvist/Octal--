#	run this with 
#   "mingw32-make.exe -f win.mk"

SRCDIR=./src

INCLUDE = C:\msys64\mingw64\include

LIBS = C:\msys64\mingw64\lib

LDFLAGS = -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -mwindows

game: main.o vec2.o physicsObject.o renderObject.o
	gcc -o game.exe main.o vec2.o physicsObject.o renderObject.o -L$(LIBS) -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -mwindows
main.o: 
	gcc -c $(SRCDIR)/main.c -I$(INCLUDE)
vec2.o: 
	gcc -c $(SRCDIR)/vec2.c
physicsObject.o: 
	gcc -c $(SRCDIR)/physicsObject.c	
renderObject.o:
	gcc -c $(SRCDIR)/renderObject.c -I$(INCLUDE)
clean:
	rm *.o
	rm *.exe