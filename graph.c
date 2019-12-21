/*!
 * \file graphe.c
 * \brief Initialisation du graphe et 
 * lancement du programme.
 * \author PANCHALINGAMOORTHY Gajenthran
 * \date 2 Décembre 2020
 */
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "parser.h"
#include "graph.h"
#include "driver.h"
#include "dijkstra.h"

graph_t _gr;

static void callback(void* d) {
  const driver_t* dr = (driver_t *)d;
  dr->keydown();
  if(dr->in->quit)
    dr->quit();

  fstack_t ** paths = NULL;
  paths = dijkstra(dr->graph, dr->in->vertex);
  dr->draw(paths, dr->in->vertex);
}

static void init_values(char * f, int model) {
  _gr.data = tokenize(f, &_gr.nbn);
  _gr.model = model;
  if(model == LIS_E) {
    _gr.vec = generate_list(_gr.data, _gr.nbn);    
  } else {
    _gr.mat = generate_mat(_gr.data, _gr.nbn);    
  }
}

void init_graph(const driver_t* dr, char * filename, int model) {
  char * f = read_file(filename);
  normalize(f);
  init_values(f, model);

  dr->init(&_gr);
  dr->start(callback);
}