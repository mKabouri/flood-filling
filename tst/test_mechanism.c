#include "test_mechanism.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

void print_matrix_colors(enum color_t *matrix_colors,int n) {

  int count = 0;
  for (int i = 0; i < n*n; i++){
    if(matrix_colors[i] != NO_COLOR){
      printf("%i ", matrix_colors[i]);
    }
    else {
      printf("0 ");
    }
    count++;
    if (count == n ) { 
      printf("\n");
      count = 0;
    }
  }
  printf("\n");
}

void testing__add_to_domain(void) {

    struct domain dom;
    dom.position = 1;
    add_to_domain(15, &dom);
    assert(dom.domain[1] == 15);
    assert(dom.position == 2);
    printf("%s: \033[0;32mpassed", __func__);   
    printf("\033[0m \n");
}


void testing__apply_game(void){

  struct graph_t *graph = board(2,'c');
  choose_adjacence('c',graph,2);
  gsl_spmatrix_uint* tmp =copy_matrix(graph,2);
  gsl_spmatrix_uint* tmp1= gsl_spmatrix_uint_compress(tmp, GSL_SPMATRIX_CSR);
  gsl_spmatrix_uint_free(graph->t);
  graph->t=tmp1;
  enum color_t matrix_colors[4]={1,2,2,3};
  print_matrix_colors(matrix_colors, 2);
  struct move_t color={.c=2};
  apply_game(0, color, graph, matrix_colors);
  print_matrix_colors(matrix_colors, 2);
  printf("%s: \033[0;32mpassed", __func__);   
  printf("\033[0m \n");

  free_gsl_graph(graph);
  gsl_spmatrix_uint_free(tmp);
}


void testing__propagate(void) {
    
  struct graph_t *graph = board(2,'c');
  choose_adjacence('c',graph,2);
  gsl_spmatrix_uint* tmp = copy_matrix(graph,2);
  gsl_spmatrix_uint* tmp1= gsl_spmatrix_uint_compress(tmp, GSL_SPMATRIX_CSR);
  gsl_spmatrix_uint_free(graph->t);
  graph->t = tmp1;
  enum color_t matrix_colors[4] = {1,2,2,3};
  struct domain dom = {.domain={0},.position=1};
  struct move_t color = {.c=2};
  propagate(0, color, &dom, graph, matrix_colors);
  assert(dom.position == 3);
  assert(dom.domain[1] == 1);

  printf("%s: \033[0;32mpassed", __func__);   
  printf("\033[0m \n");

  free_gsl_graph(graph);
  gsl_spmatrix_uint_free(tmp);

}