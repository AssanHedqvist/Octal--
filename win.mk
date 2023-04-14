#	run this with 
#   "mingw32-make.exe -f win.mk"

SRCDIR=./src

INCLUDE = C:\msys64\mingw64\include

LIBS = C:\msys64\mingw64\lib

LDFLAGS = -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -mwindows

game: main.o vec2.o integration.o object.o
	gcc -o game.exe main.o vec2.o integration.o object.o -L$(LIBS) -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -mwindows
main.o: 
	gcc -c $(SRCDIR)/main.c -I$(INCLUDE)
vec2.o: 
	gcc -c $(SRCDIR)/vec2.c
integration.o: 
	gcc -c $(SRCDIR)/integration.c	
object.o:
	gcc -c $(SRCDIR)/object.c -I$(INCLUDE)
clean:
	rm *.o
	rm *.exe