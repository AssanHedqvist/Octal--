#	linux users fix this
#	run this with 
#   "make -f linux.mk"

SRCDIR=./src
INCLUDE=./include
INCLUDE2 = /usr/include/SDL2
LDFLAGS = -lSDL2main -lSDL2 -lSDL2_image -lm

CFLAGS = -g -c

game: main.o vec2.o
	gcc -o game.exe main.o vec2.o $(LDFLAGS)
main.o: 
	gcc -c $(SRCDIR)/main.c 
vec2.o: 
	gcc -c $(SRCDIR)/vec2.c
	
clean:
	rm -f *.exe *.o