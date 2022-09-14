// Gtk 3 is needed to build this code
//
// percolation.c
// by Juan Keymer
//
// to compile type:
// gcc percolation.c -Wall -o percolate `pkg-config --cflags gtk+-3.0` `pkg-config --libs>
//
//  or just use the makefile
//
// LIBS
#include <gtk/gtk.h> 	        // GUI, Gtk Lib
#include "mt64.h"				// Pseudo Random Number Generation 64bit MT Lib	
#include <math.h>				// math to transform random n from continuos to discrete
#include <time.h>				// time library to initialize random generator with time seed



// MACROS						// to later defince a 2D Latice of 256
#define X_SIZE 256
#define Y_SIZE 256

// STRUCTURE with the DATA
struct percolation_map
	{
	int lattice_configuration[X_SIZE][Y_SIZE];      // Latice configuration
	float  percolation_probability;
	} s;	// instance s of the structure to hold the simulation


// GDK:
// Declare PUT PIXEL function to access individual pixel data on a Pixel Buffer. Implemented at the end of document.
void put_pixel(GdkPixbuf *pixbuf, int x, int y, guchar red, guchar green, guchar blue, guchar alpha);

// Pain a background function to make a pixel buffern and an image to display as default canvas
static void paint_a_background (gpointer data)
	{
	GdkPixbuf *p;
	p = gdk_pixbuf_new(GDK_COLORSPACE_RGB, 0, 8, X_SIZE, Y_SIZE);
	/*Paint a background canvas for start up image*/
  	int x,y;
	for (x = 0; x <256; x++)
	      for (y = 0; y < 256; y++)
			put_pixel(p, (int)x, (int)y, (guchar)x, (guchar)y, (guchar)x+y, 255);
	gtk_image_set_from_pixbuf(GTK_IMAGE(data), GDK_PIXBUF(p));
	g_object_unref(p);
	}

// Function to paint lattice DATA into a pixbuffer
static void paint_lattice (gpointer data)
	{
   	GdkPixbuf *p;
	p = gdk_pixbuf_new(GDK_COLORSPACE_RGB, 0, 8, X_SIZE, Y_SIZE);
	// Paint the lattice configuration to a pixbuffer
	int x,y;
	for (x = 0; x < X_SIZE; x++)
		for (y = 0; y < Y_SIZE; y++)
			if(s.lattice_configuration[x][y]==0)
				// VACANCY is painted WHITE
				{put_pixel(p, (int)x, (int)y, (guchar)255, (guchar)255, (guchar)255, 255);
				}else{
				// OCCUPANCY is painted BLACK
			 	put_pixel(p, (int)x, (int)y, (guchar)0, (guchar)0, (guchar)0, 255);
				}
	gtk_image_set_from_pixbuf(GTK_IMAGE(data), GDK_PIXBUF(p));
	g_object_unref(p);
	}


// CALL BACK to initialize the lattice button click
static void percolate_lattice(GtkWidget *widget, gpointer data)
        {
        int x,y;
        //Start with en empty lattice
        for (x = 0; x < X_SIZE; x++)
                for (y = 0; y < Y_SIZE; y++) s.lattice_configuration[x][y]=0;
//                        s.occupancy = 0;

           // Initialize with a  10% lattice occupancy at random
                               for (x = 0; x < X_SIZE; x++)
                for (y = 0; y < Y_SIZE; y++) 
                                if(genrand64_real2() < s.percolation_probability){s.lattice_configuration[x][y]=1;}
paint_lattice (data);
}



//  CALL BACK to respond Gtk SCALE SLIDE move event
static void probability_scale_moved (GtkRange *range, gpointer  user_data)
        {
        GtkWidget *label = user_data;
        gdouble pos = gtk_range_get_value (range);
        s.percolation_probability = (float) pos;
        gchar *str = g_strdup_printf ("p = %.2f", pos);
        gtk_label_set_text (GTK_LABEL (label), str);
        g_free(str);
        }

	



// HERE GOES THE ABOUT DIALOG BOX For info at a website: lab wiki on the contact process
static void show_about(GtkWidget *widget, gpointer data) 
        {
        GdkPixbuf *pixbuf = gdk_pixbuf_new_from_file("kimero_LAB_transparent.tiff", NULL);
        GtkWidget *dialog = gtk_about_dialog_new(); 
        gtk_about_dialog_set_program_name (GTK_ABOUT_DIALOG(dialog),
                                  "Percolate map  Application");     
        gtk_about_dialog_set_version(GTK_ABOUT_DIALOG(dialog), "v 0.0.1, 2022"); 
        gtk_about_dialog_set_copyright(GTK_ABOUT_DIALOG(dialog),"Open Source Code");
        gtk_about_dialog_set_comments(GTK_ABOUT_DIALOG(dialog), 
     "A Percolation map: lattice sites are on with a given probability");
        gtk_about_dialog_set_website(GTK_ABOUT_DIALOG(dialog), "https://github.com/jekeymer");
        gtk_about_dialog_set_logo(GTK_ABOUT_DIALOG(dialog), pixbuf);
        g_object_unref(pixbuf), pixbuf = NULL;          
        gtk_dialog_run(GTK_DIALOG (dialog));
        gtk_widget_destroy(dialog);
        }




// ACTIVATE function with all Widget Initialization and creation
static void activate (GtkApplication *app, gpointer user_data)
	{
	// declare a couple  of Gtk WIDGETS for the GUI
	GtkWidget *window, *image_lattice, *grid, *button, *separator;
	// to draw into the window images
	GdkPixbuf *pixbuf;

	// to control the parameter of the percolation map
	GtkWidget *probability_scale, *probability_label;


	//we initialize the mt algorithm for random number genration
	unsigned long long seed = (unsigned int)time(NULL);
	init_genrand64(seed);


	//define default parameters of the map
	s.percolation_probability = 0.0; // Richardson tumor growth model


	/* Create a new WINDOW, and set its title */
	window = gtk_application_window_new (app);
	gtk_window_set_title (GTK_WINDOW (window), "Percolation map");
	gtk_window_set_resizable (GTK_WINDOW(window), FALSE);


	/* Here we make a GRID that is going pack our Widgets */
	grid = gtk_grid_new ();
	/* Pack the GRID  in the window */
	gtk_container_add (GTK_CONTAINER (window), grid);




	// SCALE SLIDE BAR to set and LABEL display  mortality
	probability_scale = gtk_scale_new_with_range(GTK_ORIENTATION_HORIZONTAL,0,1,0.01);
	probability_label = gtk_label_new ("probability"); /* LABEL to be shown probability*/

        g_signal_connect (probability_scale,"value-changed", G_CALLBACK (probability_scale_moved), probability_label);


	// attach them to the grid
	gtk_grid_attach (GTK_GRID (grid), probability_scale, 0, 0, 2, 1);
	// position (0,0) spanning 2 col and 1 raw
	gtk_grid_attach (GTK_GRID (grid), probability_label, 2, 0, 3, 1);
	 // position (2,0) spanning 3 col and 1 raw


	// PIXEL BUFFER @ START UP and LATTICE CONFIGURATION DISPLAY IMAGE
	pixbuf = gdk_pixbuf_new(GDK_COLORSPACE_RGB, 0, 8, X_SIZE, Y_SIZE);
	image_lattice = gtk_image_new_from_pixbuf(pixbuf);
	paint_a_background(image_lattice);
        /* we pack the image into the grid */
	gtk_grid_attach (GTK_GRID (grid), image_lattice, 0, 1, 5, 1); 
	// position (0,1) spanning 5 col and 1 raw) 

	//separator
	separator = gtk_separator_new(GTK_ORIENTATION_HORIZONTAL);
	gtk_grid_attach (GTK_GRID (grid), separator, 0, 2, 5, 1); 
	// Buttons
	// -----    MAP BUTTON   -----
	button = gtk_button_new_with_label ("Map");
	//g_signal_connect (button, "clicked", G_CALLBACK (init_lattice), GTK_IMAGE(image_lattice)); 
	g_signal_connect (button, "clicked", G_CALLBACK (percolate_lattice), GTK_IMAGE(image_lattice)); 
	gtk_grid_attach (GTK_GRID (grid), button, 0, 3, 1, 1); // position (0,3) spanning 1 col and 1 raw) 


	// -----   CLUSTERS BUTTON   -----
	button = gtk_button_new_with_label ("Clusters");
	//g_signal_connect (button, "clicked", G_CALLBACK (start_simulation), GTK_IMAGE(image_lattice));
	gtk_grid_attach (GTK_GRID (grid), button, 1, 3, 1, 1); // position (1,3) spanning 1 col and 1 raw)

	button = gtk_button_new_with_label ("?");
	
	//g_signal_connect (button, "clicked", G_CALLBACK(show_about), GTK_WINDOW(window));
	g_signal_connect (button, "clicked", G_CALLBACK(show_about), GTK_WINDOW(window));

	// attach to grid
	gtk_grid_attach (GTK_GRID (grid), button, 2, 3, 1, 1); // position (3,3) spanning 1 col and 1 raw)
	//-------   QUIT BUTTON    ----
	button = gtk_button_new_with_label ("Quit"); 
	g_signal_connect_swapped (button, "clicked", G_CALLBACK (gtk_widget_destroy), window);
	gtk_grid_attach (GTK_GRID (grid), button, 3, 3, 1, 1); // position (4,3) spanning 1 col and 1 raw)




	// Show the window and all widgets
	gtk_widget_show_all (window);
	}










// Main
int main (int    argc, char **argv)	{
	GtkApplication *app;
	int status;
	app = gtk_application_new ("keymer.lab.pixbufer", G_APPLICATION_FLAGS_NONE);
	g_signal_connect (app, "activate", G_CALLBACK (activate), NULL);
	status = g_application_run (G_APPLICATION (app), argc, argv);
	g_object_unref (app);
	return status;
	}


// Implementation of putpixel. Thanks to code from,
// https://developer.gnome.org/gdk-pixbuf/stable/gdk-pixbuf-The-GdkPixbuf-Structure.html
void put_pixel(GdkPixbuf *pixbuf, int x, int y, guchar red, guchar green, guchar blue, guchar alpha)
	{
	guchar *pixels, *p;
	int rowstride, numchannels;
	numchannels = gdk_pixbuf_get_n_channels(pixbuf);
	rowstride = gdk_pixbuf_get_rowstride(pixbuf);
	pixels = gdk_pixbuf_get_pixels(pixbuf);
	p = pixels + y * rowstride + x * numchannels;
	p[0] = red;	p[1] = green; p[2] = blue; p[3] = alpha;
	}
