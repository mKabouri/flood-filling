#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "test_graph.h"
#include "test_mechanism.h"

int main() {
/**********************TESTING GRAPH FUNCTIONS**********************/

    testing__graph_creation();
    testing__graph_cpy();
    testing__adjacence_display();

/*******************TESTING MECHANISM FUNCTIONS********************/

    testing__add_to_domain();
    testing__propagate();
    testing__apply_game();
    return 0;
}
