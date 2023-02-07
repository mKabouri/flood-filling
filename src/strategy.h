/**
 * \file strategy.h
 * \brief game strategies
 *
 *
 */
#ifndef _STRATEGY_H_
#define _STRATEGY_H_

#include "mechanism.h"


/**
 * This function is the strategy which chooses the neighbour's colour of the player domain which has the largest union of connexe area.
 * \param id id of the player 
 * \param dom domain of the player in the game
 * \param matrix_colors colors matrix of the game
 * \param graph the player graph
 * \param forbidden_colors forbidden colors of the players 
 * \param my_color the last move made by the player
 */

struct strategy_gain strategie_connexe(int id,struct domain d, enum color_t *matrix_colors, struct graph_t* graph,struct color_set_t *forbiden_colors,struct move_t my_color);

/**
 * This function is the strategy which chooses the neighbour's color of the player domain which has the maximum occurance in the domain contains vertices limit the player domain from the rest of game domain
 */

struct strategy_gain strategie_max_neighbours(int id,struct domain d, enum color_t *matrix_colors, struct graph_t* graph,struct color_set_t *forbiden_colors,struct move_t my_color);

/**
 * This function is the strategy which anticipate the game by predecting future moves to win.
 */

struct move_t super_strategie(int id ,struct graph_t *graph,enum color_t *colors,int size,struct color_set_t *forbiden_colors);

#endif //_STRATEGY_
