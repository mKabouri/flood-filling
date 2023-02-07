#ifndef __TEST_MECHANISM_H__
#define __TEST_MECHANISM_H__
#include "../src/mechanism.h"
#include "../src/color.h"
#include "../src/graph.h"

//////////////////////////////////////////////////////

extern struct graph_t *board(size_t size,char shape);

extern void choose_adjacence(char shape,struct graph_t *graph, size_t size);

extern void choose_colorate(int colorate,enum color_t COL[], enum color_t colors[MAX_COLOR],
                            size_t size, size_t hole_diameter, char shape);

extern void free_gsl_graph(struct graph_t *graph);

extern gsl_spmatrix_uint* copy_matrix(struct graph_t *src,int size) ;

//////////////////////////////////////////////////////



//////////////////////////////////////////////////////

void testing__add_to_domain(void);
void testing__propagate(void) ;
void testing__apply_game(void) ;

#endif // __TEST_FILE_H__
