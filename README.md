Percolation App

The App is an application of Percolation Theory in the (256 x 256) square 2D lattice using Gtk3 to generate a Gdk Pixbuffer.

It implements the Hoshel-Kopelman algorithm,

https://en.wikipedia.org/wiki/Hoshen%E2%80%93Kopelman_algorithm#Percolation_theory

to detect clusters. This implementation used here is thanks to:

https://gist.github.com/tobin/909424

The code from this gist is mostly encapsulated in the "myHK.h" file.


To compile:

gcc percolation.c -lm -o percolate `pkg-config --cflags gtk+-3.0` `pkg-config --libs gtk+-3.0`


or: just type make.
