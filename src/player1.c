#include "player.h"
#include "strategy.h"
#include "mechanism.h"

struct player {

  char * name;
  size_t id;
  struct graph_t* graph;
  enum color_t* colors;
  struct color_set_t forbidden[NUM_PLAYERS];
  int score;
};

struct player player1 = {.name="MOUHCINE_MOHAMED_YASSINE"};

char const* get_player_name() {

  return player1.name;
}


void initialize(size_t id, struct graph_t* graph,
		enum color_t* colors,
		struct color_set_t forbidden[NUM_PLAYERS]) {

  player1.id = id;
  
  player1.graph = graph;

  player1.colors = colors;
  
  /* for (long unsigned int i = 0; i < graph->num_vertices; i++) { */

  /*   player1.colors[i] = colors[i]; */
  /* } */

  for (int i = 0; i < NUM_PLAYERS; i++) {
    for (int j = 0; j < MAX_COLOR; j++) {
      player1.forbidden[i].t[j] = forbidden[i].t[j];
    }
  }
}

unsigned int c[8] = {16711680, 255, 65280, 16776960,16753920, 15631086, 65535, 16761035};
//char *ColorString[9] = {"NO_COLOR", "BLUE", "RED", "GREEN", "YELLOW", "ORANGE", "VIOLET", "CYAN", "PINK"};


struct move_t play(struct move_t previous_move) {
  if (previous_move.c != NO_COLOR){
    apply_game(1-player1.id, previous_move, player1.graph, player1.colors);
  }
  struct move_t play={.c=NO_COLOR};
  struct move_t my_color={.c=player1.colors[player1.graph->positions[player1.id]]};
  struct domain d=player_domain(player1.id, player1.graph, my_color, player1.colors);

  struct strategy_gain strat = strategie_max_neighbours(player1.id,d,player1.colors,player1.graph,player1.forbidden,my_color);
  play.c=strat.color;
  apply_game(player1.id, play, player1.graph, player1.colors);
  return play;
}
      

int score() {
  
  return player1.score;
}


void finalize() {
  gsl_spmatrix_uint_free(player1.graph->t);
  free(player1.graph);
  free(player1.colors);
}


