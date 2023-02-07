#include <dlfcn.h>
#include <stdlib.h>
#include <stdio.h>
#include "player.h"
#include "graph.h"
#include <getopt.h>
#include <time.h>
#include "mechanism.h"

void print_matrix_colors(enum color_t *matrix_colors,unsigned int *c,int n) {
  printf("#\n");
  int count = 0;
  for (int i = 0; i < n*n; i++){
    if(matrix_colors[i] != NO_COLOR){
      printf("%i ", c[matrix_colors[i]]);
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


//////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////

extern struct graph_t *board(size_t size,char shape);

extern void choose_adjacence(char shape,struct graph_t *graph, size_t size);

extern gsl_spmatrix_uint* copy_matrix(struct graph_t *src,int size) ;
  
extern struct graph_t *graph_cpy(const struct graph_t *src, int size);

extern void free_gsl_graph(struct graph_t *graph);

extern void choose_colorate(int colorate,enum color_t matrix_colors[], enum color_t colors[MAX_COLOR], size_t size, size_t hole_diameter, char shape);

///////////////////////////////////////////




struct graph_t * convert(struct graph_t * graph) {
  graph->t = gsl_spmatrix_uint_compress(graph->t, GSL_SPMATRIX_CSR);
  return graph;
}

void print_action(int id, enum color_t previous) {
  printf("player %d played ", id);
  
  if ( previous == 0 ) {
    printf("\033[0;34m");
  }
  
  else if( previous == 1 ) {
    printf("\033[0;31m");
  }
  
  else if ( previous == 2) {
    printf("\033[0;32m");
  }

  else if ( previous == 3) {
    printf("\033[33;1m");
  }

  else if ( previous == 4) {  
    printf("\033[33m");
  }

  else if ( previous == 5) {
    printf("\033[0;35m");
  }
  
  else if ( previous == 6) {
    printf("\033[0;36m");
  }

  else if ( previous == 7) {
    printf("\033[35;1m");
  }

  else if ( previous == -1) {
    printf("\033[30;5m");
  }
  
  printf("%s \n", color_to_string(previous));
  printf("\033[0m");
}



/////////////////////////////////////////

char const * (*get_player1_name)();
char const * (*get_player2_name)();


void (*initialize_player1) (size_t id, struct graph_t* graph,
                enum color_t* colors,
                struct color_set_t forbidden[NUM_PLAYERS]);
void (*initialize_player2) (size_t id, struct graph_t* graph,
                enum color_t* colors,
                struct color_set_t forbidden[NUM_PLAYERS]);



struct move_t (*play_player1)(struct move_t previous_move);
struct move_t (*play_player2)(struct move_t previous_move);

int (*score_player1)();
int (*score_player2)();

void (*finalize_player1)();
void (*finalize_player2)();
/////////////////////////////////////////////

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
                      enum color_t *matrix_colors, 
                      struct color_set_t forbidden[NUM_PLAYERS]) {

  struct domain domain_player_id = player_domain(id, graph, chosen_move, matrix_colors);
  int length = domain_player_id.position;
  struct graph_t *graph_for_validation = graph_cpy(graph, graph_size);
  apply_game(id, chosen_move, graph_for_validation, matrix_colors);
  
  if (length == d.position) {
    printf(" Player (id = %ld) chose a color that not increase his connexe area.\n", id);
    printf(" Player (id = %ld) won the game.\n", 1-id);
    free_gsl_graph(graph_for_validation);
    return 0;
  }

  if (!check_touch(id, graph_for_validation, chosen_move, matrix_colors)) {
    printf(" Player (id = %ld) played the other player's color such that it would connect them both.\n", id);
    printf(" Player (id = %ld) won the game.\n", 1-id);
    free_gsl_graph(graph_for_validation);
    return 0;
  }

  if ( forbidden[id].t[chosen_move.c] == 1 && chosen_move.c != NO_COLOR) {
    printf(" Player (id = %ld) played forbidden color : %s.\n", id,color_to_string(chosen_move.c));
    printf(" Player (id = %ld) won the game.\n", 1-id);
    free_gsl_graph(graph_for_validation);
    return 0;
  }
  
  free_gsl_graph(graph_for_validation);
  return 1;
}

/** 
 * This function decides the winner of the game: 
 * "The winner is the one with the highest score"
 */
void winner(unsigned int id, struct graph_t *graph, enum color_t *matrix_colors){
  struct move_t player1_color={.c=matrix_colors[graph->positions[id]]};
  struct move_t player2_color={.c=matrix_colors[graph->positions[1-id]]};
  struct domain d1=player_domain(id, graph, player1_color, matrix_colors);
  struct domain d2=player_domain(1-id, graph, player2_color, matrix_colors);
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

/////////////////////////////////////////////
int main(int argc, char *argv[]) {
  srand(time(NULL));
  int param;
  int m=9;
  char t='c';
  int coloration=1;
  int forbidden_number=0;
  int sdl=0;
  while((param = getopt(argc, argv, "m:c:t:a:f:S:")) != -1){   
    switch(param){ 
      case 'm': 
        m = atoi(optarg); 
        break; 
      case 't':
        t=*optarg;
        break; 
      case 'c': 
        continue;   
        break; 
      case 'a': 
        coloration=atoi(optarg);   
        break; 
      case 'f': 
        forbidden_number=atoi(optarg);
        break; 
      case 'S': 
        sdl=atoi(optarg);
        break; 
      default: 
        printf("Argument not used by main, please use -m -t -c -a or -f or -S.\n"); 
        abort(); 
    } 
  }
  if (argc > 1) {
    void * lib = dlopen(argv[argc-2], RTLD_NOW); 
    void * lib2 = dlopen(argv[argc-1], RTLD_NOW); 

    //get player name functions.
    
    get_player1_name = dlsym(lib, "get_player_name"); 
    get_player2_name = dlsym(lib2, "get_player_name"); 

    //Initialize player functions.
    
    initialize_player1 = dlsym(lib, "initialize"); 
    initialize_player2 = dlsym(lib2, "initialize"); 
    

    //play functions.
    
    play_player1 = dlsym(lib, "play"); 
    play_player2 = dlsym(lib2, "play"); 
    //score functions.

    score_player1 = dlsym(lib, "score");
    score_player2 = dlsym(lib2, "score");

    //finalize functions.
    
    finalize_player1 = dlsym(lib, "finalize"); 
    finalize_player2 = dlsym(lib2, "finalize"); 

    if (lib == NULL || lib2 == NULL ) { 
        printf("bibliothèque vide\n"); 
        exit(EXIT_FAILURE); 
    } 


    //Initializing graph and colors.

    struct graph_t *graph = board(m,t);
    
    choose_adjacence(t,graph,m);   //function that choose the adjacense wanted according to the combination of m and t
   
    enum color_t colors[MAX_COLOR] = {BLUE, RED, GREEN, YELLOW, ORANGE, VIOLET, CYAN, PINK};
    unsigned int c[8] = {16711680, 255, 65280, 16776960,16753920, 15631086, 65535, 16761035};
    enum color_t matrix_colors[m*m];

    choose_colorate (coloration,matrix_colors, colors, m, m/3, t);

    //Forbidden colors 

    struct color_set_t forbidden[NUM_PLAYERS];
    
    char forbidden_colors[MAX_COLOR]={ 0, 0, 0, 0, 0, 0, 0, 0 };

    if(forbidden_number!=0){
      int number = rand()%MAX_COLOR;
      for (int j = 0; j < forbidden_number; j++) {
        forbidden_colors[(number)%MAX_COLOR]=1;
        number+=1;
      }
    }

    for (int j = 0; j < MAX_COLOR; j++) {
      forbidden[0].t[j]=forbidden_colors[j];
      forbidden[1].t[j]=forbidden_colors[j];
    }
    //Creation of two copies of the initialized graph and colors matrix.
    enum color_t *matrix_colors_cpy = malloc(sizeof(enum color_t)*m*m);
    for (int i = 0; i < m*m; i++) {
      matrix_colors_cpy[i] = matrix_colors[i];
    }
    enum color_t *matrix_colors_cpy2 = malloc(sizeof(enum color_t)*m*m);
    for (int i = 0; i < m*m; i++) {
      matrix_colors_cpy2[i] = matrix_colors[i];
    }
    
    gsl_spmatrix_uint* tmp =copy_matrix(graph,m);
    gsl_spmatrix_uint* tmp1= gsl_spmatrix_uint_compress(tmp, GSL_SPMATRIX_CSR);
    gsl_spmatrix_uint_free(graph->t);
    graph->t=tmp1;
    

    struct graph_t *graph_cpy1 = graph_cpy(graph, graph->t->size1);
    struct graph_t *graph_cpy2 = graph_cpy(graph, graph->t->size1);

    //Initializing players.

    int player1_id = rand()%2;
    int player2_id = 1-player1_id;
    
    initialize_player1(player1_id, graph_cpy1, matrix_colors_cpy, forbidden); 
    initialize_player2(player2_id, graph_cpy2, matrix_colors_cpy2, forbidden); 

    ////////////////////////////////////////////////////////////////////////////////////////
    /****************************LOOP OF THE GAME******************************************/

    struct move_t previous = {.c = NO_COLOR};
    previous = play_player1(previous); // we start with the player1

    struct move_t color_player1 = {.c = matrix_colors[graph->positions[player1_id]]};
    struct domain domain_player1 = player_domain(player1_id, graph, color_player1, matrix_colors);
    struct move_t color_player2 = {.c = matrix_colors[graph->positions[player2_id]]};
    struct domain domain_player2 = player_domain(player2_id, graph, color_player2, matrix_colors);
    
    int end_game = 0;
    
    if (sdl){

      /*************************** Display sdl *********************/

      printf("%i %i\n", m, m);
      print_matrix_colors(matrix_colors, c, m);
      while(end_game != 2 ){
	end_game=0;
        apply_game(player1_id, previous, graph, matrix_colors);
        print_matrix_colors(matrix_colors, c, m);
        previous=play_player2(previous);
	if (previous.c == NO_COLOR) {
          end_game++;
        }
        apply_game(player2_id, previous, graph, matrix_colors);
        print_matrix_colors(matrix_colors, c, m);
        previous=play_player1(previous);
	if (previous.c == NO_COLOR) {
          end_game++;
        }
      }
    }
    else {

      /*************************** Display terminal *********************/
      
      printf("\n player %s has (id = %i)",get_player1_name(),player1_id);
      printf("\n player %s has (id = %i)",get_player2_name(),player2_id);

      printf("\ncolors forbidden for player (id = %d) are : ",player1_id);
      for (int j = 0; j < MAX_COLOR; j++) {
        if(forbidden[player1_id].t[j]==(char)1){
          printf("%s ", color_to_string(j));
        }
      }
      printf("\ncolors forbidden for player (id = %d) are : ",1-player1_id);
      for (int j = 0; j < MAX_COLOR; j++) {
        if(forbidden[1-player1_id].t[j]==(char)1){
          printf("%s ", color_to_string(j));
        }
      }
      printf("\n\n");
      while(end_game != 2 ) {

        end_game=0;

        if (!play_valid_color(domain_player1, player1_id, graph, m*m, previous, matrix_colors, forbidden)) {
          break;
        }
        apply_game(player1_id, previous, graph, matrix_colors);
        print_action(player1_id, previous.c);

        
        previous = play_player2(previous);
        if (previous.c == NO_COLOR) {
          end_game++;
        }
        if (!play_valid_color(domain_player2, player2_id, graph, m*m, previous, matrix_colors,forbidden)) {
          break;
        }
        apply_game(player2_id, previous, graph, matrix_colors);
        print_action(player2_id, previous.c);
        
        previous = play_player1(previous);
        if (previous.c == NO_COLOR) {
          end_game++;
        }
      }

      winner(player1_id, graph, matrix_colors); 
    } 
       
    ////////////////////////////////////////////////////////////////////////////////////////

    //Finalize party.
    
    finalize_player1();
    finalize_player2();
    gsl_spmatrix_uint_free(tmp);
    free_gsl_graph(graph);

    dlclose(lib2);
    dlclose(lib);
  }   
  return 0;
}
