/*
All original code I (Juan Keymer) am playing with here comes from 
Tobin's
https://gist.github.com/tobin/909424
*/


#include <stdio.h>
#include <stdlib.h>
//#include <assert.h>



#define max(a,b) (a>b?a:b)
#define min(a,b) (a>b?b:a)



/* Implementation of Union-Find Algorithm */
// I mean, Tobin's implemtation
//
// Useful read here is:
// https://en.wikipedia.org/wiki/Disjoint-set_data_structure
//
/* The 'labels' array has the meaning that labels[x] is an alias for the label x; by
   following this chain until x == labels[x], you can find the canonical name of an
   equivalence class.  The labels start at one; labels[0] is a special value indicating
   the highest label already used. */

int *labels;
int  n_labels = 0;     /* length of the labels array */



// Find canonical member of the Equivalence class
/*  uf_find returns the canonical label for the equivalence class containing x */
int uf_find(int x)
	{
  	int y = x;
  	while (labels[y] != y)
    		y = labels[y];
	while (labels[x] != x)
		{
    		int z = labels[x];
    		labels[x] = y;
    		x = z;
  		}
  	return y;
	}


// Union
/*  uf_union joins two equivalence classes and returns the canonical label of the resulting class. */
int uf_union(int x, int y)
	{
  	return labels[uf_find(x)] = uf_find(y);
	}


// Make_set
/*  uf_make_set creates a new equivalence class and returns its label */
int uf_make_set(void)
	{
  	labels[0] ++;
	// assert here fails with only <. neds <+.
 	// assert(labels[0] < n_labels);
	//assert(labels[0] <= n_labels);
	labels[labels[0]] = labels[0];
  	return labels[0];
	}


// Init Union-Find
/*  uf_intitialize sets up the data structures needed by the union-find implementation. */
void uf_initialize(int max_labels)
	{
  	n_labels = max_labels;
  	labels = calloc(sizeof(int), n_labels);
  	labels[0] = 0;
	}

// For cleaning:
/*  uf_done frees the memory used by the union-find data structures */
void uf_done(void)
	{
  	n_labels = 0;
  	free(labels);
  	labels = 0;
	}


// function to print the matrix (input)
// ______________
/* print_matrix prints out a matrix that is set up in the "pointer to pointers" scheme
   (aka, an array of arrays); this is incompatible with C's usual representation of 2D
   arrays, but allows for 2D arrays with dimensions determined at run-time */

void print_matrix(int **matrix, int m, int n) 
	{
  	for (int i=0; i<m; i++) {
    		for (int j=0; j<n; j++)
      			printf("%3d ",matrix[i][j]);
    		printf("\n");
  		}
	}


// Here comes the actual Hoshen_Kopelman
//_
int hoshen_kopelman(int **matrix, int m, int n) 
	{
	uf_initialize(m * n / 2);

	/* scan the matrix */
 	for (int i=0; i<m; i++)
    		for (int j=0; j<n; j++)
      			if (matrix[i][j])// if occupied 
				{
				//  look up   (unless it is the first row)
				int up = (i==0 ? 0 : matrix[i-1][j]);
				//  look left (unless it is the first column)
				int left = (j==0 ? 0 : matrix[i][j-1]);
				switch (!!up + !!left) 
					{
					case 0: // If not up nor left are not-zero
	  					matrix[i][j] = uf_make_set();// a new cluster
	  					break;

					case 1: // part of an existing cluster
	  					matrix[i][j] = max(up,left);//whichever is nonzero is labelled
	  					break;

					case 2: // this site binds two clusters
	  					matrix[i][j] = uf_union(up, left);
	  					break;
					}
				}



	/* apply the relabeling to the matrix */
	/* This is a little bit sneaky.. we create a mapping from the canonical labels
	determined by union/find into a new set of canonical labels, which are
	guaranteed to be sequential. */

	int *new_labels = calloc(sizeof(int), n_labels); // allocate array, initialized to zero

	for (int i=0; i<m; i++)
    		for (int j=0; j<n; j++)
      			{
      			if (matrix[i][j])
				{
				int x = uf_find(matrix[i][j]);
				if (new_labels[x] == 0)
						{
	  					new_labels[0]++;
	  					new_labels[x] = new_labels[0];
						}
				matrix[i][j] = new_labels[x];
      				}
			}
	int total_clusters = new_labels[0];

  	free(new_labels);
  	uf_done();

  	return total_clusters;
      	}


// check labels
/* This procedure checks to see that any occupied neighbors of an occupied site
   have the same label. */
/*
void check_labelling(int **matrix, int m, int n) {
  int N,S,E,W;
  for (int i=0; i<m; i++)
    for (int j=0; j<n; j++)
      if (matrix[i][j]) {
	N = ( i==0 ? 0 : matrix[i-1][j] );
	S = ( i==m-1 ? 0 : matrix[i+1][j] );
	E = ( j==n-1 ? 0 : matrix[i][j+1] );
	W = ( j==0 ? 0 : matrix[i][j-1] );
	
	assert( N==0 || matrix[i][j]==N );
	assert( S==0 || matrix[i][j]==S );
	assert( E==0 || matrix[i][j]==E );
	assert( W==0 || matrix[i][j]==W );
      }
}
*/




