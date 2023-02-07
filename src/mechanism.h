/**
 * \file mechanism.h
 * \brief functions needed by server and players
 *
 *
 */
#ifndef _MECHANISM_H_
#define _MECHANISM_H_

#include "graph.h" 

#define TAILLE 1000

/**
 * This structure represents the possible colors to play with and their count.
 */

struct possible {
  enum color_t possible[MAX_COLOR];
  int num;
};

/**
 * This structure represents the vertexes owned by the player and their count.
 */

struct domain {
  unsigned int domain[TAILLE];
  int position;
};

/**
 * This structure represents the color and the number of vertexes gained by playing it.
 */

struct strategy_gain {
  enum color_t color;
  int gain;
};

/**
 * This function calculate the connexe area of a vertix given.
 */

int num_connexe_area(int id, size_t s, struct domain dom, struct graph_t * graph, enum color_t * matrix_colors);

/**
 * This function returns the index of the biggest number in the array t.
 */

int max_tab(int t[], int n);

/**
 * This function verifies if the value a exists in the array d.
 */

int exist_in_tab(unsigned int *d, int size,unsigned int a);

/**
 * This function counts the number of occurence of the value val in the array tab .
 */

int count_occurence(int *tab,int size,int val);

/**
 * This function adds a new vertix to the domain dom.
 */

void add_to_domain (unsigned int vertix,struct domain *dom);

/**
 * This function checks if players domains are neighbours.
 */

int check_touch(int id, struct graph_t *graph, struct move_t color, enum color_t *matrix_colors) ;

/**
 * This function propagate the given colour .
 */

void propagate(unsigned int vertix,struct move_t color,struct domain *dom, struct graph_t *graph, enum color_t matrix_colors[]) ;

/**
 * This function colorate the domain of the player with the color given.
 */

void colorate_domain(struct domain dom,struct move_t color,enum color_t matrix_colors[]);

/**
 * This function updates the game state by applying the chosen color by the player with (id).
 */

void apply_game(int id, struct move_t color, struct graph_t *graph, enum color_t *matrix_colors) ;

/**
 * This function gets the player's domain.
 */

struct domain player_domain(int id, struct graph_t *graph, struct move_t color, enum color_t *matrix_colors) ;

/**
 * this function create a copy of the graph coloration array.
 */

enum color_t* copy_COL(enum color_t *colors, int m);

/**
 * This function evaluate a giving move.
 */

int evaluate_move(int id,struct graph_t *graph,enum color_t *colors,struct move_t move ,int size,int depth,struct color_set_t *forbiden_colors);

/**
 * This function gathers all the possible colors to be played.
 */

struct possible possible_moves( int id, struct graph_t *graph, enum color_t *colors,struct color_set_t *forbiden_colors);

#endif // _MECHANISM_H_
