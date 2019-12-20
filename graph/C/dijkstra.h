#ifndef DIJKSTRA_H_
#define DIJKSTRA_H_

#include "list.h"
#include "mat.h"
#include "fstack.h"
#include "graph.h"

fstack_t ** dijkstra(const graph_t * graph, int dep);
fstack_t ** dijkstra_lis(vec_t v, int dep); 
fstack_t ** dijkstra_mat(mat_t m, int dep);
void        print_paths(fstack_t ** paths, int nbpaths, int dep);
void        rewind_paths(fstack_t ** paths, int n);
int         my_rand(int min, int max);
#endif
