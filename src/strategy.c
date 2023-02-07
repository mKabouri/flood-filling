#include "strategy.h"

////////// MIX_MAX STRATEGY ///////////////


struct move_t super_strategie(int id ,struct graph_t *graph,enum color_t *colors,int size,struct color_set_t *forbiden_colors) {
  struct possible p=possible_moves(id,graph,colors,forbiden_colors);
  struct move_t result = {.c=NO_COLOR};
  int max=-1000000;
  if (p.num == 0) return result;
  int *outcome=malloc(sizeof(*outcome)*MAX_COLOR);
  for (int i=0;i<MAX_COLOR;i++){
    if (p.possible[i]!= -1){
      struct move_t next={.c=p.possible[i]};
      outcome[i]=evaluate_move( id, graph, colors, next ,size,0,forbiden_colors);
      if(outcome[i] >= max ) {
	      max=outcome[i];
	      result.c=p.possible[i];
      }
    }
  }
  free(outcome);
  return result;
}
/////////////////////  MAX_CONNEXE_AREA      /////////////

struct strategy_gain strategie_connexe(int id,struct domain dom, enum color_t *matrix_colors, struct graph_t* graph,struct color_set_t *forbiden_colors,struct move_t my_color){
  struct strategy_gain strat={.color=NO_COLOR};
  int color_count[MAX_COLOR];
  for(int i = 0; i < MAX_COLOR; i++) {
    color_count[i] = 0;
  }
   
  struct move_t current_color = {.c = NO_COLOR};
  
  for (int s=0 ; s < dom.position ;s++) {
    for (int k = graph->t->p[dom.domain[s]]; k < graph->t->p[dom.domain[s]+1]; k++) {
      
      unsigned int j = graph->t->i[k];
      current_color.c = matrix_colors[j];
      if ((matrix_colors[j] != my_color.c)
	    && check_touch(id, graph, current_color, matrix_colors)
      && forbiden_colors->t[matrix_colors[j]]==0 ) {
	
	    color_count[matrix_colors[j]] =  num_connexe_area(id, j, dom, graph, matrix_colors);
      }
    }
  }
  strat.color=max_tab(color_count, MAX_COLOR);
  strat.gain=color_count[strat.color];
  if (strat.gain == 0) strat.color = NO_COLOR; 
  return strat;
}


////////// MAX_NEIGHBOURS //////////////


struct strategy_gain strategie_max_neighbours(int id,struct domain dom, enum color_t *matrix_colors, struct graph_t* graph,struct color_set_t *forbiden_colors,struct move_t my_color){

  struct strategy_gain strat={.color=NO_COLOR};
  int possible_colors[TAILLE];
  unsigned int neighbors_checked[TAILLE];
  int i = 0;
  struct move_t current_color = {.c = NO_COLOR};
  
  // getting a list of possible colors to play
  for (int s = 0 ;s < dom.position; s++) {
    for (int k = graph->t->p[dom.domain[s]]; k < graph->t->p[dom.domain[s]+1]; k++) {
      unsigned int j = graph->t->i[k];
      current_color.c = matrix_colors[j];
      if ((matrix_colors[j] != my_color.c) && !exist_in_tab(neighbors_checked,i,j) 
	      && check_touch(id, graph, current_color, matrix_colors)&& forbiden_colors->t[matrix_colors[j]]==0) {	
        neighbors_checked[i] = j;
        possible_colors[i] = matrix_colors[j];
	i++;
      }
    }
  }
  // looking for the best color to play from neighbours 
  if(i != 0) {
    int max=0;
    int best_color=0;
    int s=0;
    for (int color=0;color<8;color++){
      s = count_occurence(possible_colors,i,color);
      if (max < s){
	      max=s;
	      best_color=color;
      }
    }
    strat.color = best_color;
    strat.gain=max;
  }
  return strat;
}


