#	run this with 
#   "mingw32-make.exe -f win.mk"

SRCDIR=./src

INCLUDE = C:\msys64\mingw64\include

LIBS = C:\msys64\mingw64\lib

LDFLAGS = -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_net -lSDL2_ttf -lSDL2_mixer -mwindows

game: main.o vec2.o physicsObject.o renderObject.o keyboard.o player.o text.o attacks.o sounds.o menu.o mouse.o
	gcc -o game.exe -O2 main.o vec2.o physicsObject.o renderObject.o keyboard.o player.o text.o attacks.o sounds.o menu.o mouse.o -L$(LIBS) $(LDFLAGS)  -Wall
main.o: 
	gcc -c $(SRCDIR)/main.c -I$(INCLUDE) -Wall
vec2.o: 
	gcc -c $(SRCDIR)/vec2.c -Wall
keyboard.o:
	gcc -c $(SRCDIR)/keyboard.c -I$(INCLUDE) -Wall
physicsObject.o: vec2.o 
	gcc -c $(SRCDIR)/physicsObject.c -Wall
renderObject.o:
	gcc -c $(SRCDIR)/renderObject.c -I$(INCLUDE) -Wall
player.o: keyboard.o renderObject.o physicsObject.o
	gcc -c $(SRCDIR)/player.c	
text.o:
	gcc -c $(SRCDIR)/text.c -I$(INCLUDE) -Wall
attacks.o:
	gcc -c $(SRCDIR)/attacks.c -I$(INCLUDE) -Wall
sounds.o:
	gcc -c $(SRCDIR)/sounds.c -I$(INCLUDE) -Wall
menu.o:
	gcc -c $(SRCDIR)/menu.c -I$(INCLUDE) -Wall
mouse.o:
	gcc -c $(SRCDIR)/mouse.c -I$(INCLUDE) -Wall

clean:
	rm *.o
	rm *.exe