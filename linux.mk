#	linux users fix this
#	run this with 
#   "make -f linux.mk"

SRCDIR=./src

INCLUDE=./include

CFLAGS = -g -c

game: main.o vec2.o
	gcc -o game.exe main.o vec2.o
main.o: 
	gcc -c $(SRCDIR)/main.c 
vec2.o: 
	gcc -c $(SRCDIR)/vec2.c
	
clean:
	rm *.exe
	rm *.o