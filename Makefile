CC = gcc
CFLAGS = -Wall -Wextra -std=c2x -pedantic -ggdb
SRC = src/*.c
LIBS = 

basm: $(SRC)
	$(CC) -o basm $(SRC) $(CFLAGS) $(LIBS)

