CC= gcc

CFLAGS= -W -Wall -lm -std=c89 -pedantic

sources= ./src/file.c ./src/mon_erreur.c ./src/liste.c ./src/pile.c ./src/arbre.c ./src/compression.c ./src/occurence.c

objets= $(sources:.c=.o)


clean : huff_v0 huff_v1 huff_v2 huff_v3
	rm -r ./src/*.o

huff_v0: ./src/huff_v0.c $(objets)
	$(CC) $(CFLAGS) -o $@  $^

huff_v1: ./src/huff_v1.c $(objets)
	$(CC) $(CFLAGS) -o $@  $^

huff_v2: ./src/huff_v2.c $(objets)
	$(CC) $(CFLAGS) -o $@  $^

huff_v3: ./src/huff_v3.c $(objets)
	$(CC) $(CFLAGS) -o $@  $^

%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $< 