all:
	gcc -Wall percolation.c mt64.c -lm -o percolate `pkg-config --cflags gtk+-3.0` `pkg-config --libs gtk+-3.0`

