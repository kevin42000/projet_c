CC= gcc

CFLAGS= -W -Wall -lm -std=c89 -pedantic

sources= ./src/file.c ./src/mon_erreur.c ./src/liste.c ./src/pile.c ./src/arbre.c ./src/compression.c ./src/occurence.c

objets= $(sources:.c=.o)


clean : main huffmanv1 huffmanv2
	rm -r ./src/*.o

main: ./src/main.c $(objets)
	$(CC) $(CFLAGS) -o $@  $^

huffmanv1: ./src/huff_v0.c $(objets)
	$(CC) $(CFLAGS) -o $@  $^

huffmanv2: ./src/huff_v1.c $(objets)
	$(CC) $(CFLAGS) -o $@  $^

%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $< 