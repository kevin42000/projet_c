NFLAGS= `pkg-config --libs-only-l MLV`

CC= gcc

CFLAGS= -W -Wall -lm -std=c89 -pedantic `pkg-config --cflags MLV` `pkg-config --libs-only-other --libs-only-L MLV`

sources= ./src/arbre.c ./src/compression.c ./src/main.c ./src/occurence.c

objets= $(sources:.c=.o)

clean : projet
	rm -r ./*.o ./src/*.o

projet: $(objets)
	$(CC) $(CFLAGS) -o $@  $^ $(NFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -o $@ $(NFLAGS) -c $< 