
INCLUDE = C:\msys64\mingw64\include
LDFLAGS = -lmingw32 -lSDL2main -lSDL2 -lSDL2_net -lm

server: server.c
	gcc -o server server.c $(LDFLAGS)
clean:
	rm *.exe