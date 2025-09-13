flags= -O2 -Wall -std=c2x -I.

.PHONY: cleanmake

all: main

main: main.o lexer_class.o
	gcc $(flags) $^ -o $@

lexer_class.o: lexer_class.c
	gcc $(flags) -c $^ -o $@

clean: 
	rm -rf *.o main