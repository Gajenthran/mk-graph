#ifndef DIJKSTRA_H_
#define DIJKSTRA_H_

#include "list.h"
#include "fstack.h"

fstack_t ** dijkstra(vec_t v, int dep);
fstack_t ** find_paths(vec_t v, int dep, int * dist, int * pred);
void        print_paths(fstack_t ** paths, int nbpaths, int dep);
void        print_pcc(fstack_t ** paths, vec_t v, int dep);
void        rewind_paths(fstack_t ** paths, int n);

#endif
