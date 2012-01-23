CC = gcc

CFLAGS = -std=c99 -Wall -Wextra -pedantic -g $(shell sdl-config --cflags)
LDFLAGS = $(shell sdl-config --libs) -lSDL_gfx -lSDL_ttf

main: helpers.o game.o menu.o highscore.o main.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o spaceinvaders helpers.o game.o menu.o highscore.o main.o

main.o: main.c
	$(CC) $(CFLAGS) -c main.c

menu.o: menu.c
	$(CC) $(CFLAGS) -c menu.c

helpers.o: helpers.c
	$(CC) $(CFLAGS) -c helpers.c

game.o: game.c
	$(CC) $(CFLAGS) -c game.c

highscore.o: highscore.c
	$(CC) $(CFLAGS) -c highscore.c

clean:
	rm -f *.o
	rm -f spaceinvaders