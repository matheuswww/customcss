flags= -O2 -Wall -std=c2x -I.

.PHONY: clean

all: main

main: main.o lexer_class.o  lexer_customcss.o
	gcc $(flags) $^ -o $@

lexer_class.o: lexer_class.c
	gcc $(flags) -c $^ -o $@

lexer_customcss.o: lexer_customcss.c
	gcc $(flags) -c $^ -o $@

clean: 
	rm -rf *.o main