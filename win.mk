#	run this with 
#   "mingw32-make.exe -f win.mk"

SRCDIR=./src

#	can i do this?
INCLUDE=./include

INCLUDE2 = C:\msys64\mingw64\include

LIBS = C:\msys64\mingw64\lib

game: main.o vec2.o
	gcc -o game.exe main.o vec2.o
main.o: 
	gcc -c $(SRCDIR)/main.c 
vec2.o: 
	gcc -c $(SRCDIR)/vec2.c
	
clean:
	rm *.exe
	rm *.o