#ifndef __TEST_GRAPH_H__
#define __TEST_GRAPH_H__
#include "../src/graph.h"

//////////////////////////////////////////////////////
extern struct graph_t *board(size_t size,char shape);

extern void choose_adjacence(char shape,struct graph_t *graph, size_t size);

extern gsl_spmatrix_uint* copy_matrix(struct graph_t *src,int size) ;
  
extern struct graph_t *graph_cpy(const struct graph_t *src, int size);

extern void free_gsl_graph(struct graph_t *graph);

extern void  make_hole(struct graph_t * graph, size_t size,  size_t m, size_t n, size_t diametre);
/////////////////////////////////////////////////////

void testing__graph_creation(void);

void testing__graph_cpy(void);

void testing__adjacence_display(void);

#endif // __TEST_FILE_H__