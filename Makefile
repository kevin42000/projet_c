CC = gcc

FLAGS = -W -Wall -std=c89 -pedantic -O3

all: huffman

huffman: main.o occurence.o arbre.o
	$(CC) $(FLAGS) obj/main.o obj/occurence.o obj/arbre.o -o bin/huffman

main.o: main.c
	$(CC) $(FLAGS) src/main.c -c obj/main.o

occurence.o: occurence.c occurence.h
	$(CC) $(FLAGS) src/occurence.c -c obj/occurence.o

arbre.o: arbre.c arbre.h
	$(CC) $(FLAGS) src/arbre.c -c obj.arbre.o

clean:
	rm -f bin/huffman obj/*.o
