CC = gcc
CFLAGS = -Wall -Wextra -Wno-unused-variable -Wno-format -std=c11 -pedantic -ggdb
SRC = src/*.c
LIBS = 

basm: $(SRC)
	$(CC) -o basm $(SRC) $(CFLAGS) $(LIBS)

