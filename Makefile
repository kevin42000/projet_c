CC = gcc

FLAGS = -W -Wall -std=c89 -pedantic -O3

all: huffman clean

huffman: main.o occurence.o arbre.o compression.o
	$(CC) $(FLAGS) obj/main.o obj/occurence.o obj/arbre.o obj/compression.o -o bin/huffman

main.o: src/main.c
	$(CC) $(FLAGS) src/main.c -c
	mv main.o obj/main.o

occurence.o: src/occurence.c inc/occurence.h
	$(CC) $(FLAGS) src/occurence.c -c
	mv occurence.o obj/occurence.o

arbre.o: src/arbre.c inc/arbre.h
	$(CC) $(FLAGS) src/arbre.c -c
	mv arbre.o obj/arbre.o

compression.o: src/compression.c inc/compression.h
	$(CC) $(FLAGS) src/compression.c -c
	mv compression.o obj/compression.o

clean:
	rm -f obj/*.o

cleanexe:
	rm -f bin/huffman