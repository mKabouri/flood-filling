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

struct player player3 = {.name="Laarbi_AbdSlam"};

char const* get_player_name() {

  return player3.name;
}


void initialize(size_t id, struct graph_t* graph,
                enum color_t* colors,
                struct color_set_t forbidden[NUM_PLAYERS]) {

  player3.id = id;
  
  player3.graph = graph;
  
  player3.colors = colors;

  player3.forbidden = malloc(sizeof(*forbidden)*NUM_PLAYERS);

  for (int i = 0; i < NUM_PLAYERS; i++) {
    for (int j = 0; j < MAX_COLOR; j++) {
      player3.forbidden[i].t[j] = forbidden[i].t[j];
    }
  }
}


struct move_t play(struct move_t previous_move) {

  if (previous_move.c != NO_COLOR){
    apply_game(1-player3.id, previous_move, player3.graph, player3.colors);
  }
  
  struct move_t play={.c=NO_COLOR};


  play = super_strategie(player3.id ,player3.graph,player3.colors,player3.graph->t->size1,player3.forbidden);
  apply_game(player3.id, play, player3.graph, player3.colors);
  return play;
}

void finalize() {
  free(player3.forbidden);
  gsl_spmatrix_uint_free(player3.graph->t);
  free(player3.graph);
  free(player3.colors);
}



