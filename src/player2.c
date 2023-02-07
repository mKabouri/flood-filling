#include "player.h"
#include "strategy.h"
#include "mechanism.h"

struct player {

  char * name;
  size_t id;
  struct graph_t* graph;
  enum color_t* colors;
  struct color_set_t *forbidden;
  int score;
};

struct player player2 = {.name="HICHAM_ATMAN"};

char const* get_player_name() {

  return player2.name;
}


void initialize(size_t id, struct graph_t* graph,
                enum color_t* colors,
                struct color_set_t forbidden[NUM_PLAYERS]) {

  player2.id = id;
  
  player2.graph = graph;
  
  player2.colors = colors;

  player2.forbidden = malloc(sizeof(*forbidden)*NUM_PLAYERS);
  for (int i = 0; i < NUM_PLAYERS; i++) {
    for (int j = 0; j < MAX_COLOR; j++) {
      player2.forbidden[i].t[j] = forbidden[i].t[j];
    }
  }
}

struct move_t play(struct move_t previous_move) {

  if (previous_move.c != NO_COLOR){
    apply_game(1-player2.id, previous_move, player2.graph, player2.colors);
  }
  struct move_t my_color = {.c = player2.colors[player2.graph->positions[player2.id]]};
  struct domain d = player_domain(player2.id, player2.graph, my_color, player2.colors);  
  
  struct move_t play={.c=NO_COLOR};
  struct strategy_gain strat=strategie_connexe(player2.id,d,player2.colors,player2.graph,player2.forbidden,my_color);
  
  play.c=strat.color;
  apply_game(player2.id, play, player2.graph, player2.colors);
  return play;
}

int score() {

  return player2.score;
}

void finalize() {

  free(player2.forbidden);
  gsl_spmatrix_uint_free(player2.graph->t);
  free(player2.graph);
  free(player2.colors);
}
