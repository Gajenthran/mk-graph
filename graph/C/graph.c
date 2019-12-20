#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "parser.h"
#include "graph.h"
#include "driver.h"
#include "dijkstra.h"

graph_t g;

static void callback(void* d) {
  const driver_t* dr = (driver_t *)d;
  dr->keydown();
  if(dr->in->quit)
    dr->quit();

  fstack_t ** paths = NULL;
  paths = dijkstra(dr->graph, dr->in->vertex);
  dr->draw(paths, dr->in->vertex);
}

void init_graph(const driver_t* dr, char * filename) {
  int n;
  char * f = read_file(filename);
  normalize(f);
  g.data = tokenize(f, &n);
  g.model = 1;
  g.vec = generate_list(g.data, n);
  dr->init(&g);
  dr->start(callback);
}