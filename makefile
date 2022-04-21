CC = gcc

FLAGS = -W -Wall -std=c89 -pedantic -O3

all: huffman

huffman: main.o
	$(CC) $(FLAGS) obj/main.o -o bin/huffman

main.o: main.c
	$(CC) $(FLAGS) src/main.c -c obj/main.o


clean:
	rm -f bin/huffman obj/*.o
