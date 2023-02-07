#ifndef _SERVER_TOOLS_H_
#define _SERVER_TOOLS_H_

#include "color.h"
#include "graph.h"
#include "mechanism.h"

//print the world matrix with rgb colors.
void print_COL(enum color_t *COL, unsigned int *c, int n) ;

//Convert COO to CSR.
struct graph_t * convert(struct graph_t * graph) ;


/**
 * This function allows the server to validate 
 * the play made by the player.
 * id: player id.
 */
int play_valid_color( struct domain d,
                      size_t id,
                      struct graph_t *graph,
                      size_t graph_size,
                      struct move_t chosen_move,
                      enum color_t *COL,
                      size_t size_forbidden[NUM_PLAYERS],  
                      struct color_set_t forbidden[NUM_PLAYERS]) ;

/** 
 * This function decides the winner of the game: 
 * "The winner is the one with the highest score"
 */
void winner(unsigned int id, struct graph_t *graph, enum color_t *COL) ;



#endif // _SERVER_TOOLS_H_