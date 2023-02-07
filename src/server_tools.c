#include "server_tools.h"

void print_COL(enum color_t *COL, unsigned int *c, int n) {
  printf("#\n");
  int count = 0;
  for (int i = 0; i < n*n; i++) {
    if(COL[i] != NO_COLOR){
      printf("%i ", c[COL[i]]);
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

struct graph_t * convert(struct graph_t * graph) {
  graph->t = gsl_spmatrix_uint_compress(graph->t, GSL_SPMATRIX_CSR);
  return graph;
}

int play_valid_color( struct domain d,
                      size_t id,
                      struct graph_t *graph,
                      size_t graph_size,
                      struct move_t chosen_move,
                      enum color_t *COL,
                      size_t size_forbidden[NUM_PLAYERS],  
                      struct color_set_t forbidden[NUM_PLAYERS]) {

  struct domain domain_player_id = player_domain(id, graph, chosen_move, COL);
  int length = domain_player_id.position;
  struct graph_t *graph_for_validation = graph_cpy(graph, graph_size);
  apply_game(id, chosen_move, graph_for_validation, COL);
  
  if (length == d.position) {
    printf(" Player (id = %ld) chose a color that not increase his connexe area.\n", id);
    printf(" Player (id = %ld) won the game.\n", 1-id);
    free_gsl_graph(graph_for_validation);
    return 0;
  }

  if (!check_touch(id, graph_for_validation, chosen_move, COL)) {
    printf(" Player (id = %ld) played the other player's color such that it would connect them both.\n", id);
    printf(" Player (id = %ld) won the game.\n", 1-id);
    free_gsl_graph(graph_for_validation);
    return 0;
  }

  for (size_t i = 0; i < size_forbidden[0]; i++) {
    if (chosen_move.c == forbidden[id].t[i]) {
      printf(" Player (id = %ld) played forbidden color.\n", id);
      printf(" Player (id = %ld) won the game.\n", 1-id);
      free_gsl_graph(graph_for_validation);
      return 0;
    }
  }
  free_gsl_graph(graph_for_validation);
  return 1;
}

/** 
 * This function decides the winner of the game: 
 * "The winner is the one with the highest score"
 */
void winner(unsigned int id, struct graph_t *graph, enum color_t *COL){
  printf("\n\n##################\n");
  printf("Game ended\n");
  struct move_t player1_color={.c=COL[graph->positions[id]]};
  struct move_t player2_color={.c=COL[graph->positions[1-id]]};
  struct domain d1=player_domain(id, graph, player1_color, COL);
  struct domain d2=player_domain(1-id, graph, player2_color, COL);
  if(d2.position < d1.position ){
    printf("\n we have a winner:\n_-_-_-_-_-_-_-_-_-%s_-_-_-_-_-_-_-_-_-\n%s scored %i and %s scored %i\n",get_player1_name(),get_player1_name(),d1.position,get_player2_name(),d2.position);
  }
  else if(d2.position == d1.position){
    printf("\nwoooow it's a draw\n");
  }
  else{
    printf("\n we have a winner:\n_-_-_-_-_-_-_-_-_-%s_-_-_-_-_-_-_-_-_-\n%s scored %i and %s scored %i\n",get_player2_name(),get_player2_name(),d2.position,get_player1_name(),d1.position);
  }
}