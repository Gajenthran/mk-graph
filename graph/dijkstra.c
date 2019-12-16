#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "dijkstra.h"

#define INFINITY 9999
static fstack_t ** find_paths(vec_t v, int dep, int * dist, int * pred);
static fstack_t ** find_paths_mat(mat_t m, int dep, int * dist, int * pred);

/**
 * \brief L'algorithme de Dijkstra permettant de calculer
 * le plus court chemin d'un point à un autre, en stockant
 * les différents chemins et leurs coût dans une pile de
 * données.
 *
 * \param v vecteur de liste de successeurs (graphe)
 * \param dep vertex de départ
 * \return pile des plus courts chemins selon un vertex
 * de départ
 */
fstack_t ** dijkstra(vec_t v, int dep) {
  int ** cost = (int **)malloc(v.nbn * sizeof(*cost));
  assert(cost);

  int i, j;
  for(i = 0; i < v.nbn; i++) {
    cost[i] = (int *)malloc(v.nbn * sizeof(*cost[i]));
    assert(cost[i]);
    for(j = 0; j < v.nbn; j++) {
      cost[i][j] = INFINITY;
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
    min_d = INFINITY;
    next_n = -1;

    for(i = 0; i < v.nbn; i++) {
      if(dist[i] < min_d && !dejavu[i]) {
        min_d = dist[i];
        next_n = i;
      }
    }

    if(next_n >= 0) {
      dejavu[next_n] = 1;
      for(i = 0; i < v.nbn; i++) {
        if(!dejavu[i]) {
          if(min_d + cost[next_n][i] < dist[i]) {
            dist[i] = min_d + cost[next_n][i];
            pred[i] = next_n;
          }
        }
      }
    }
    c++;
  }

  return find_paths(v, dep, dist, pred);
}

fstack_t ** dijkstra_mat(mat_t m, int dep) {
  int ** cost = (int **)malloc(m.nbn * sizeof(*cost));
  assert(cost);

  int i, j;
  for(i = 0; i < m.nbn; i++) {
    cost[i] = (int *)malloc(m.nbn * sizeof(*cost[i]));
    assert(cost[i]);
    for(j = 0; j < m.nbn; j++) {
      cost[i][j] = INFINITY;
    }
  }

  int dist[m.nbn], pred[m.nbn];
  int dejavu[m.nbn], c, min_d, next_n;

  i = 0;
  while (i < m.nba) {
    do {
      cost[m.c[i].n][m.c[i].s] = m.c[i].val;
      i++;
    } while(i < m.nba && m.c[i].n == m.c[i-1].n);
  }

  for(i = 0; i < m.nbn; i++) {
    dist[i] = cost[dep][i];
    pred[i] = dep;
    dejavu[i] = 0;
  }

  dist[dep] = 0;
  dejavu[dep] = 1;
  c = 1;

  while(c < m.nbn - 1) {
    min_d = INFINITY;
    next_n = -1;

    for(i = 0; i < m.nbn; i++) {
      if(dist[i] < min_d && !dejavu[i]) {
        min_d = dist[i];
        next_n = i;
      }
    }

    if(next_n >= 0) {
      dejavu[next_n] = 1;
      for(i = 0; i < m.nbn; i++) {
        if(!dejavu[i]) {
          if(min_d + cost[next_n][i] < dist[i]) {
            dist[i] = min_d + cost[next_n][i];
            pred[i] = next_n;
          }
        }
      }
    }
    c++;
  }

  return find_paths_mat(m, dep, dist, pred);
}

/**
 * \brief Rempiler les différents chemins 
 * des vertex qui ont été dépilés.
 *
 * \param paths liste des chemins pour un vertex donné
 * \param n     nombre de chemins
 */
void rewind_paths(fstack_t ** paths, int n) {
  int i;
  for(i = 0; i < n; i++) {
    rewind_stack(paths[i]);
  }
}

/**
 * \brief Place les différents plus cours chemin
 * d'un vertex par rapport aux autres vertices
 * en stockant le chemin et le coût de chaque
 * chemin.
 *
 * \param v vecteur de liste de successeurs (graphe)
 * \param dep vertex de départ
 * \param dist distance entre un vertex et les autres
 * \param pred vertex prédit pour arriver au vertex final
 * \return pile des plus courts chemins selon un vertex
 * de départ
 */
static fstack_t ** find_paths(vec_t v, int dep, int * dist, int * pred) {
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

/**
 * \brief Place les différents plus cours chemin
 * d'un vertex par rapport aux autres vertices
 * en stockant le chemin et le coût de chaque
 * chemin, utilisant une matrice compacte.
 *
 * \param v vecteur de liste de successeurs (graphe)
 * \param dep vertex de départ
 * \param dist distance entre un vertex et les autres
 * \param pred vertex prédit pour arriver au vertex final
 * \return pile des plus courts chemins selon un vertex
 * de départ
 */
static fstack_t ** find_paths_mat(mat_t m, int dep, int * dist, int * pred) {
  fstack_t ** paths;
  paths = (fstack_t **)malloc(m.nbn * sizeof(*paths));
  assert(paths);
  int i, j;
  for(i = 0; i < m.nbn; i++) {
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

/** \brief Fonction retournant un nombre aléatoire entre deux
 * intervalles.
 *
 * \param min intervalle minimal
 * \param max intervalle maximal
 * 
 * \return Nombre aléatoire entre les intervalles min-max.
 */
int my_rand(int min, int max) {
  return (rand()/(double)RAND_MAX) * (max - min) + min;
}
