#	run this with 
#   "make -f mac.mk"

# Location of where main.c is stored in
SRCDIR=./src

# Name of the compiler, GNU GCC in this case
CC=gcc

# Any include directories
INCLUDE = /opt/homebrew/include

# Any flags for the compiler
CFLAGS = -g -I$(INCLUDE) -c
#CFLAGS = -g -c

# Any libraries to link with
#	LIBS = /usr/local/lib
LIBS = /opt/homebrew/lib


# Extra flags to give to compilers when they are supposed to invoke the linker
#LDFLAGS = -lmingw32 -lSDL2main -lSDL2 -mwindows
LDFLAGS = -lSDL2main -lSDL2 -lSDL2_image -lSDL2_net -lSDL2_ttf -lSDL2_mixer

game: main.o vec2.o physicsObject.o renderObject.o keyboard.o player.o text.o attacks.o sounds.o menu.o mouse.o
	gcc -o game main.o vec2.o physicsObject.o renderObject.o keyboard.o player.o text.o attacks.o sounds.o menu.o mouse.o -L$(LIBS) -lSDL2main -lSDL2 -lSDL2_image -lSDL2_net -lSDL2_ttf -lSDL2_mixer
main.o: 
	gcc -c $(SRCDIR)/main.c -I$(INCLUDE)
vec2.o: 
	gcc -c $(SRCDIR)/vec2.c
keyboard.o:
	gcc -c $(SRCDIR)/keyboard.c -I$(INCLUDE)
physicsObject.o: 
	gcc -c $(SRCDIR)/physicsObject.c	
renderObject.o:
	gcc -c $(SRCDIR)/renderObject.c -I$(INCLUDE)
player.o: keyboard.o renderObject.o physicsObject.o
	gcc -c $(SRCDIR)/player.c	
text.o:
	gcc -c $(SRCDIR)/text.c -I$(INCLUDE)
attacks.o:
	gcc -c $(SRCDIR)/attacks.c -I$(INCLUDE)
sounds.o:
	gcc -c $(SRCDIR)/sounds.c -I$(INCLUDE)
menu.o:
	gcc -c $(SRCDIR)/menu.c -I$(INCLUDE)
mouse.o:
	gcc -c $(SRCDIR)/mouse.c -I$(INCLUDE)

clean:
	rm -f *.o
	rm -f main