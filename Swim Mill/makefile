all: Fish Pellet Swim_Mill
Fish: Fish.o
	gcc -o Fish Fish.o
Fish.o: Fish.c
	gcc -c Fish.c -o Fish.o -std=c99
Pellet: Pellet.o
	gcc -o Pellet Pellet.o
Pellet.o: Pellet.c
	gcc -c Pellet.c -o Pellet.o -std=c99
Swim_Mill: Swim_Mill.o
	gcc -o Swim_Mill Swim_Mill.o
Swim_Mill.o: Swim_Mill.c
	gcc -c Swim_Mill.c -o Swim_Mill.o -std=c99
