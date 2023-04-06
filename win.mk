#	run this with 
#   "mingw32-make.exe -f win.mk"

SRCDIR=./src

#	can i do this?
INCLUDE=./include

INCLUDE2 = C:\msys64\mingw64\include

LIBS = C:\msys64\mingw64\lib

game: main.o
	gcc -o game.exe main.o
main.o:
	gcc -c $(SRCDIR)/main.c
	
clean:
	rm *.exe
	rm *.o