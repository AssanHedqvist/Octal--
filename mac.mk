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
LIBS = /opt/homebrew/lib


# Extra flags to give to compilers when they are supposed to invoke the linker
#LDFLAGS = -lmingw32 -lSDL2main -lSDL2 -mwindows
LDFLAGS = -lSDL2main -lSDL2 -lSDL2_image

main:
	@echo "Building main"
	$(CC) $(CFLAGS) $(SRCDIR)/main.c -o $@.o 
	$(CC) main.o -o main $(LDFLAGS) -L$(LIBS)

clean:
	rm -f main.o
	rm -f main