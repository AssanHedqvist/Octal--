#	linux users fix this
#	run this with 
#   "make -f linux.mk"

SRCDIR=./src

CFLAGS = -g -c

game: main.o
	gcc -o game.exe main.o
	
main.o:
	gcc -c $(SRCDIR)/main.c

clean:
	rm  main.o
	rm  main