Percolation App

The App is an application of Percolation Theory in the square 2D lattice.

It implements the Hoshel-Kopelman algorithm,to detect clusters. This implementation is thanks to:

https://gist.github.com/tobin/909424

The code from this gist is mostly encapsulated in the "myHK.h" file.

compile line

gcc percolation.c -lm -o percolate `pkg-config --cflags gtk+-3.0` `pkg-config --libs gtk+-3.0`


or: just type make.
