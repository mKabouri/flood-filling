#include "mechanism.h"


struct possible possible_moves( int id, struct graph_t *graph, enum color_t *colors,struct color_set_t *forbiden_colors) {

  struct move_t my_color = {.c=colors[graph->positions[id]]};
  struct domain d = player_domain(id, graph, my_color, colors);
  struct possible p = {.possible={-1,-1,-1,-1,-1,-1,-1,-1}};
  int i = 0;
  struct move_t current_color = {.c = NO_COLOR};
  for (int s = 0; s < d.position; s++) {
    for (int k = graph->t->p[d.domain[s]]; k < graph->t->p[d.domain[s]+1]; k++) {

      unsigned int j = graph->t->i[k];
      current_color.c = colors[j];
      if ((colors[j] != my_color.c) && check_touch(id, graph, current_color, colors) && p.possible[colors[j]] == -1 
           && forbiden_colors->t[colors[j]] == 0){
        p.possible[colors[j]] = colors[j];
        i++;
      }
    }
  }
  p.num = i;
  return p;
}

enum color_t* copy_COL(enum color_t *colors, int m) {

  enum color_t *cpy = malloc(sizeof(enum color_t)*m);
  for (int i = 0; i < m; i++) {
    cpy[i] = colors[i];
  }
  return cpy;
}

int is_terminal(int id,struct graph_t *graph,enum color_t *colors,struct move_t move,int size,struct color_set_t *forbiden_colors) {

  struct domain new_d = player_domain(id, graph, move, colors);
  enum color_t *cpy = copy_COL(colors,size);
  apply_game(id, move, graph, cpy);
  struct possible p = possible_moves(1-id,graph,cpy,forbiden_colors);
  struct move_t color_o = {.c=colors[graph->positions[1-id]]};
  struct domain other_d = player_domain(1-id, graph, color_o, cpy);
  free(cpy);
  if (p.num == 0 && new_d.position >= other_d.position) {

    return 1;
  }
  return 0;
}


int evaluate_move(int id,struct graph_t *graph,enum color_t *colors,struct move_t move ,int size,int depth,struct color_set_t *forbiden_colors) {

  if (is_terminal(id, graph, colors, move, size,forbiden_colors)) return 1000000;

  if (depth == 3 ) return 0;

  int res = 0;
  struct domain new_dom=player_domain(id, graph, move, colors);
  enum color_t *cpy=copy_COL(colors,size);
  apply_game(id, move, graph, cpy);
  struct possible p=possible_moves(1-id,graph,cpy,forbiden_colors);
  for (int i=0; i < MAX_COLOR; i++){
    if (p.possible[i] != -1){
      enum color_t *s_cpy=copy_COL(cpy,size);
      struct move_t next={.c=p.possible[i]};
      struct domain dom=player_domain(1-id, graph, next, s_cpy);
      res+=new_dom.position-dom.position-evaluate_move(1-id, graph, s_cpy, next, size,depth+1,forbiden_colors);
      free(s_cpy);
    }
  }
  free(cpy);
  return res;
}


int count_occurence(int *tab,int size,int val){
  int rslt=0;
  for (int i=0;i<size;i++){
    if (tab[i]==val){
      rslt++;
    }
  }
  return rslt;
}

int exist_in_tab(unsigned int *d, int size,unsigned int a) {
  for (int i=0;i<size;i++) {
    if (d[i] == a) return 1;
  }
  return 0;
}

int max_tab(int t[], int n) {
  int max = 0;
  for(int i = 1; i < n; i++) {
    if(t[max] < t[i]) {
      max = i;
    }
  }
  return max;
}

void add_to_domain (unsigned int s, struct domain *d){
    d->domain[d->position]=s;
    d->position++;
}


int check_touch(int id, struct graph_t *graph, struct move_t color, enum color_t *COL) {
  struct domain d = player_domain(id, graph, color, COL);
  for(int i = 0; i < d.position; i++) {
    if (d.domain[i] == graph->positions[1-id]) {
      return 0;
    }
  }
  return 1;
} 


int num_connexe_area(int id, size_t s, struct domain d, struct graph_t * graph, enum color_t * COL) {
  int actual_size=d.position;
  struct move_t color={.c=COL[s]};
  struct domain new_d=player_domain(id, graph, color, COL);
  return new_d.position-actual_size;
}

void propagate(unsigned int s, struct move_t color, struct domain *d, struct graph_t *graph, enum color_t *COL) {
  if (COL[s] != COL[d->domain[0]] && COL[s] != color.c) return ;
  for (int k = graph->t->p[s]; k < graph->t->p[s+1]; k++) {
    unsigned int j = graph->t->i[k];
    if ( COL[j] == COL[d->domain[0]] && COL[s] != COL[d->domain[0]]   ) (void)(s);
    else if(exist_in_tab(d->domain,d->position,j) == 0 && (COL[j] == color.c || COL[j] == COL[d->domain[0]] )){
      add_to_domain(j,d); 
      propagate(j,color,d,graph,COL);
    }
  }
}

void colorate_domain(struct domain d, struct move_t color, enum color_t COL[]) {
  for (int i=0;i<d.position;i++) {
    COL[d.domain[i]]=color.c;
  }
}

struct domain player_domain(int id, struct graph_t *graph, struct move_t color, enum color_t *COL) {
  struct domain d={.domain={graph->positions[id]},.position=1};
  propagate(d.domain[0], color, &d, graph, COL);
  return d;
}


void apply_game(int id, struct move_t color, struct graph_t *graph, enum color_t *COL) {
  if (color.c != NO_COLOR){
    struct domain d= player_domain(id, graph, color, COL);
    colorate_domain(d,color,COL);
  }
}


//////////////////////////////////////////// strategies ////////////////////////////////////////////////////////////////
