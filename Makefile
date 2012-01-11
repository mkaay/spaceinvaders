CC = gcc 
CFLAGS = -std=c99 -Wall -Wextra -pedantic -D_GNU_SOURCE=1 -D_THREAD_SAFE -Wl,-framework,Cocoa -g
INCLUDE = -I/usr/local/include/SDL
LDFLAGS = -L/usr/local/lib -lSDLmain -lSDL -lSDL_gfx

main: main.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o spaceinvaders main.o

main.o: main.c
	$(CC) $(CFLAGS) $(INCLUDE) -c main.c
