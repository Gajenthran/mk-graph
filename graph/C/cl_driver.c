#include "graph.h"
#include "driver.h"
#include <stdlib.h>

static int      init(const graph_t* graph);
static void     start(void(*callback)(void*));
static void     quit_game(void);

driver_t cl_driver = {
    .graph = NULL,
    .init = init,
    .start = start,
    .quit_game = quit_game
};


#define GRAPH (cl_driver.graph)

static int init(const graph_t* graph) {
    GRAPH = graph;
    return 0;
}

static void start(void(*callback)(void*)) {
    for(;;) {
        
    }
}

static void quit_game(void) {
    exit(0);
}

