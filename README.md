Percolation App

The App is an application of Percolation Theory

https://en.wikipedia.org/wiki/Percolation_theory

in the (256 x 256) square 2D lattice using Gtk3 to generate a Gdk Pixbuffer.



For identifying Lattice animals(clusters) it implements the Hoshel-Kopelman algorithm,

https://en.wikipedia.org/wiki/Hoshen%E2%80%93Kopelman_algorithm#Percolation_theory

to detect clusters. The algorithm is a spatial version of the Union-Find algorithm or Disjoint-Set structure

https://en.wikipedia.org/wiki/Disjoint-set_data_structure

The implementation used here is thanks to Tobin Fricke:

https://www.ocf.berkeley.edu/~fricke/projects/hoshenkopelman/hoshenkopelman.html

https://gist.github.com/tobin/909424

The code from the gist link above, here it is mostly encapsulated in the "myHK.h" file.




To implement a simple gradient colormap, it build from ideas discussed at,

https://stackoverflow.com/questions/20792445/calculate-rgb-value-for-a-range-of-values-to-create-heat-map


To compile:

gcc percolation.c -lm -o percolate `pkg-config --cflags gtk+-3.0` `pkg-config --libs gtk+-3.0`


or: just type make.
