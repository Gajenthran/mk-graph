/**
 * Pointeur de fonctions pour SDL
 * français/anglais ?
 * static/extern ?
 * pcc
 * pointeur de fonction
 * remove the infinity
 * remove ll
 * argv
 */

/*
 * - Représentation d'un labyrinthe -
 * Voici la modélisation d'un labyrinthe à travers un graphe orienté.
 * Chaque salle donne accès (ou non) à une autre salle, il est parfois
 * impossible de retourner dans la salle précédente. Chaque déplacement
 * vers une salle consomme de l'énergie. Le but est de consommer le
 * moins d'énergie possible pour passer d'une salle à une autre.
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "fstack.h"
#include "list.h"
#include "parser.h"


fstack_t ** dijkstra(vec_t v, int dep);
fstack_t ** find_paths(vec_t v, int dep, int * dist, int * pred);
void        print_paths(fstack_t ** paths, int nbpaths, int dep);
void        print_pcc(fstack_t ** paths, vec_t v, int dep);
void        rewind_paths(fstack_t ** paths, int n);

int main(void) {
  int n;
  char * file = read_file("maze.txt");
  normalize(file);
  data_t * data = tokenize(file, &n);
  vec_t v = generate_list(data, n);
  print_name_list(v);
  fstack_t ** paths = dijkstra(v, 6);
  print_pcc(paths, v, 6);
  return 0;
}

void rewind_paths(fstack_t ** paths, int n) {
  int i;
  for(i = 0; i < n; i++) {
    rewind_stack(paths[i]);
  }
}


fstack_t ** dijkstra(vec_t v, int dep) {
  int ** cost = (int **)malloc(v.nbn * sizeof(*cost));
  assert(cost);
  int i, j;
  for(i = 0; i < v.nbn; i++) {
    cost[i] = (int *)malloc(v.nbn * sizeof(*cost[i]));
    assert(cost[i]);
    for(j = 0; j < v.nbn; j++) {
      cost[i][j] = 9999;
    }
  }

  int dist[v.nbn], pred[v.nbn];
  int dejavu[v.nbn], c, min_d, next_n;

  for(i = 0; i < v.nbn; i++) {
    list_t * l = v.n[i].l;
    while(l) {
      cost[i][l->an->in] = l->v;
      l = l->succ;
    }
  }

  for(i = 0; i < v.nbn; i++) {
    dist[i] = cost[dep][i];
    pred[i] = dep;
    dejavu[i] = 0;
  }

  dist[dep] = 0;
  dejavu[dep] = 1;
  c = 1;

  while(c < v.nbn - 1) {
    min_d = 9999;

    for(i = 0; i < v.nbn; i++) {
      if(dist[i] < min_d && !dejavu[i]) {
        min_d = dist[i];
        next_n = i;
      }
    }

    dejavu[next_n] = 1;
    for(i = 0; i < v.nbn; i++) {
      if(!dejavu[i]) {
        if(min_d + cost[next_n][i] < dist[i]) {
          dist[i] = min_d + cost[next_n][i];
          pred[i] = next_n;
        }
      }
    }
    c++;
  }

  return find_paths(v, dep, dist, pred);
}

void print_paths(fstack_t ** paths, int nbpaths, int dep) {
  int i;
  for(i = 0; i < nbpaths; i++) {
    printf("dir:   %d to %d\n", dep, paths[i]->index);
    printf("cost:  %d\n", paths[i]->dist);
    printf("paths: ");
    if(i != dep) {
      while(!empty_stack(paths[i])) {
        printf("%d -> ", pop_stack(paths[i]));
      }
    }
    printf("FIN \n\n\n");
  }
}

void print_pcc(fstack_t ** paths, vec_t v, int dep) {
  int i;
  for(i = 0; i < v.nbn; i++) {
    printf("start:   %s to %s\n", v.n[dep].name, v.n[paths[i]->index].name);
    printf("cost:  %d\n", paths[i]->dist);
    if(i != dep) {
      while(!empty_stack(paths[i])) {
        printf("%s -> ", v.n[pop_stack(paths[i])].name);
      }
    }
    printf("FIN \n\n\n");
  }
}

fstack_t ** find_paths(vec_t v, int dep, int * dist, int * pred) {
  fstack_t ** paths;
  paths = (fstack_t **)malloc(v.nbn * sizeof(*paths));
  assert(paths);
  int i, j;
  for(i = 0; i < v.nbn; i++) {
    paths[i] = init_stack(i, dist[i]);
    if(i != dep) {
      push_stack(paths[i], i);
      j = i;
      do {
        j = pred[j];
        push_stack(paths[i], j);
      } while(j != dep);
    }
  }
  return paths; 
}