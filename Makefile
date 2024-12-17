CC = gcc
CFLAGS = -Wall -Wextra
all: game

game: 2048.o game.o
	$(CC) $(CFLAGS) -o game 2048.o game.o -lncurses

2048.o: 2048.c game.h
	$(CC) $(CFLAGS) -c 2048.c

game.o: game.c game.h
	$(CC) $(CFLAGS) -c game.c

# Run the game
run: game
	./game

clean:
	rm -f *.o game
