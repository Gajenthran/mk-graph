#ifndef DIJKSTRA_H_
#define DIJKSTRA_H_

#include "list.h"
#include "fstack.h"

fstack_t ** dijkstra(vec_t v, int dep);
void        print_paths(fstack_t ** paths, int nbpaths, int dep);
void        rewind_paths(fstack_t ** paths, int n);
int         my_rand(int min, int max);
#endif
