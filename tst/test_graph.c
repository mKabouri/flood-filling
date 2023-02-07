#include "test_graph.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

void testing__graph_creation(void) {

    struct graph_t *graph = board(20,'c');
    assert(graph->num_vertices==400);
    assert(graph->positions[0]==0);
    assert(graph->positions[1]==399);
    printf("\nGraph creation test : \033[0;32mpassed");   
    printf("\033[0m \n");
    free_gsl_graph(graph);
}

void testing__graph_cpy(void) {

    struct graph_t *graph = board(20, 'd');
    
    gsl_spmatrix_uint* tmp =copy_matrix(graph, 20);
    gsl_spmatrix_uint* tmp1= gsl_spmatrix_uint_compress(tmp, GSL_SPMATRIX_CSR);
    gsl_spmatrix_uint_free(graph->t);
    graph->t=tmp1;

    struct graph_t *cp_graph = graph_cpy(graph, graph->t->size1);
    assert(graph->num_vertices == cp_graph->num_vertices);
    assert(graph->positions[0] == cp_graph->positions[0]);
    assert(graph->positions[1] == cp_graph->positions[1]);

    //Changing a value in graph->t.
    graph->t->i[graph->t->p[0]] = 5;
    cp_graph->t->i[cp_graph->t->p[0]] = 10;
    assert(graph->t->i[graph->t->p[0]] == 5);
    assert(cp_graph->t->i[graph->t->p[0]] == 10);
    //Comme l'assert passe alors cp_graph est une copie.

    printf("%s: \033[0;32mpassed", __func__);   
    printf("\033[0m \n");

    gsl_spmatrix_uint_free(tmp);
    free_gsl_graph(graph);
    free_gsl_graph(cp_graph);
}

void testing__adjacence_display(void) {

    printf("\n Adjacence display :\n");

    struct graph_t *graph = board(15,'c');

    choose_adjacence('c',graph,15);

    printf("\n Matrice d'adjacence carré :\n\n");

    for (int i=0;i<15;i++){
       for (int j=0;j<15;j++){
            printf(" %u", gsl_spmatrix_uint_get(graph->t,i,j));
        }
        printf("\n");
    }

    choose_adjacence('h',graph,15);

    printf("\n Matrice d'adjacence forme H :\n\n");

    for (int i=0;i<15;i++){
       for (int j=0;j<15;j++){
            printf(" %u", gsl_spmatrix_uint_get(graph->t,i,j));
        }
        printf("\n");
    }

    choose_adjacence('d',graph,15);

    printf("\n Matrice d'adjacence donut :\n\n");

    for (int i=0;i<15;i++){
       for (int j=0;j<15;j++){
            printf(" %u", gsl_spmatrix_uint_get(graph->t,i,j));
        }
        printf("\n");
    }

    choose_adjacence('t',graph,15);

    printf("\n Matrice d'adjacence torique :\n\n");

    for (int i=0;i<15;i++){
       for (int j=0;j<15;j++){
            printf(" %u", gsl_spmatrix_uint_get(graph->t,i,j));
        }
        printf("\n");
    }
    printf("%s: \033[0;32mpassed", __func__);   
    printf("\033[0m \n");
    free_gsl_graph(graph);
}
