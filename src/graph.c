/**
 * \file graph.c
 *
 *
 */
#include <stdio.h>
#include "graph.h"
#include <stdlib.h>
#include <time.h>



int colors[9] = {16711680, 255, 65280, 16776960,
  16753920, 15631086, 65535, 16761035};


gsl_spmatrix_uint *initial_board(size_t size){
  gsl_spmatrix_uint *board=gsl_spmatrix_uint_alloc(size,size);
  return board;
}

/**
 * This function creates the graph corresponding to the shape chosed.
 */

struct graph_t *board(size_t size,char shape){
  struct graph_t *board=malloc(sizeof(struct graph_t));
  board->t=initial_board(size*size);
  int num_vertice;
  int carre=size*size;
  switch(shape){
    case 'c':
    case 't':
      num_vertice=carre;
      break;
    case 'h':
      num_vertice=7*carre/9;
      break;
    case 'd':
      num_vertice=8*carre/9;
      break;
  }
  board->num_vertices=num_vertice;
  board->positions[0]=0;
  board->positions[1]=size*size-1;
  return board;
}



////////////////////////////////////////////// main ///////////////////////////////////////////////////

void fill_column(gsl_spmatrix_uint * mat, size_t size, size_t column, size_t value) {
  for(size_t i = 0; i < size; i++) {
    gsl_spmatrix_uint_set(mat, i, column, value);
  }
}


/**
 * This function creates the adjacence matrix of square graph.
 */

void adjacence_square(struct graph_t *graph, size_t size ) {
  for (size_t i=0;i<size*size;i++) {
    if ((int)(i-size)>=0) {
      gsl_spmatrix_uint_set(graph->t,i,i-size,1);
    }
    if (i+size<size*size) {
      gsl_spmatrix_uint_set(graph->t,i,i+size,1);
    }
    if (i%size != 0) {
      gsl_spmatrix_uint_set(graph->t,i,i-1,1);
    }
    if ((i+1)%size!=0 ) {
      gsl_spmatrix_uint_set(graph->t,i,i+1,1);
    }
  }
}    

/**
 * This function creates a hole in the graph.
 */

void  make_hole(struct graph_t * graph, size_t size,  size_t m, size_t n, size_t diametre) {
  size_t radius = (diametre - 1)/2;
  size_t row_first = m - radius;
  size_t col_first = n - radius;
  size_t row = row_first;
  size_t col = col_first;

  for(size_t i = 0; i < diametre ; i++) {
    row = row_first + i ;

    for(size_t j = 0; j < diametre ; j++) {
      col = col_first + j;
      fill_column(graph->t, size*size, size*row + col, 0);
    }
  }
  gsl_spmatrix_uint_set(graph->t, m, n, 0);
}


void adjacence_donut(struct graph_t * graph, size_t size, size_t hole_diameter) {
  adjacence_square(graph, size);
  if(hole_diameter != 0) {
    make_hole(graph, size, size/2, size/2, hole_diameter);
  }
}

void adjacence_H(struct graph_t * graph, size_t size, size_t hole_diameter) {
  adjacence_square(graph, size);
  if(hole_diameter != 0) {  
    size_t row_center1 = hole_diameter / 2;
    size_t row_center2 = size - (hole_diameter / 2 ) - 1;
    size_t col_center = size / 2;
    make_hole(graph, size, row_center1, col_center, hole_diameter);
    make_hole(graph, size, row_center2, col_center, hole_diameter);
  }
}


void adjacence_torique(struct graph_t * graph, size_t size) {
  adjacence_square(graph, size);
  size_t V_step = size*(size-1);
  size_t H_step = size-1;
  for (size_t i = 0; i < size; i++) {
    gsl_spmatrix_uint_set(graph->t, i, V_step+i, 1);
    gsl_spmatrix_uint_set(graph->t, V_step+i, i, 1);
  }
  
  for (size_t i = 0; i < size*size; i += size) {
    gsl_spmatrix_uint_set(graph->t, i, i + H_step, 1);
    gsl_spmatrix_uint_set(graph->t, i + H_step, i, 1);
  }

}

/**
 * This function creates a copy of the graph matrix.
 */

gsl_spmatrix_uint* copy_matrix(struct graph_t *src,int size) {
  gsl_spmatrix_uint *copy=gsl_spmatrix_uint_alloc(src->t->size1,src->t->size2);
  for (unsigned int i = 0; i <size*size ; i++) {
    for (unsigned int k = 0; k < size*size; k++) {
      if (gsl_spmatrix_uint_get(src->t,i,k) == 1){
	      gsl_spmatrix_uint_set(copy, i, k, 1);
      }
    }
  }
  return copy;
}

/**
 * This function chooses the adjacence matrix according to the shape choosed.
 */

void choose_adjacence(char shape,struct graph_t *graph, size_t size){
  if (size%3 != 0 && (shape == 'd' || shape == 'h')){
    printf("\nit's not possible to get the shape choosed with the size choosed, please choose a size multiple of 3...\n");
    abort();
  }
  
  switch (shape) {
  case 'c':
    adjacence_square(graph,size );
    break;
  case 'd':
    adjacence_donut(graph,size,size/3 );
    break;
  case 'h':
    adjacence_H(graph,size,size/3 );
    break;
  case 't':
    adjacence_torique(graph, size);
    break;
  }
}

/**
 * This function creates a copy of the graph.
 */

struct graph_t *graph_cpy(const struct graph_t *src, int size) {

  struct graph_t *graph_dst = malloc(sizeof(struct graph_t));
  gsl_spmatrix_uint *tmp=initial_board(size);
  graph_dst->t=gsl_spmatrix_uint_compress(tmp, GSL_SPMATRIX_CSR);
  gsl_spmatrix_uint_free(tmp);
  gsl_spmatrix_uint_memcpy(graph_dst->t, src->t); 
  graph_dst->num_vertices = src->num_vertices;
  graph_dst->positions[0] = src->positions[0];
  graph_dst->positions[1] = src->positions[1];
  return graph_dst;
}

/**
 * This function frees the allocated memory to graph.
 */

void free_gsl_graph(struct graph_t *graph) {
  gsl_spmatrix_uint_free(graph->t);
  free(graph);
}
