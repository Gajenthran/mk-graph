/*!
 * \file dijkstra.h
 * \brief Header du fichier dijkstra.c
 * \author PANCHALINGAMOORTHY Gajenthran
 * \date 2 Décembre 2020
 */
#ifndef DIJKSTRA_H_
#define DIJKSTRA_H_

#include "list.h"
#include "mat.h"
#include "fstack.h"
#include "graph.h"

fstack_t ** dijkstra(const graph_t*, int);
fstack_t ** dijkstra_lis(const graph_t*, int); 
fstack_t ** dijkstra_mat(const graph_t*, int);
fstack_t ** find_paths(int, int, int *, int *);
void        print_paths(fstack_t **, int, int);
void        rewind_paths(fstack_t **, int);
int         my_rand(int, int);

#endif
