CC = cc
CFLAGS = -std=c11 -Wall -Wextra -pedantic

all: formes

formes: main.c
	$(CC) $(CFLAGS) main.c -o formes

run: formes
	./formes

clean:
	rm -f formes dessin.svg projet_formes.txt

.PHONY: all run clean
