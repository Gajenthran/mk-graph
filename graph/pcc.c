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
#include "sdl_driver.h"

int main(void) {
  int n;
  char * file = read_file("maze.txt");
  normalize(file);
  data_t * data = tokenize(file, &n);
  vec_t v = generate_list(data, n);
  print_name_list(v);
  fstack_t ** paths = NULL;  
  /* int i, j;
  for(i = 0; i < v.nbn; i++) {
    fstack_t ** paths = dijkstra(v, i);
    print_pcc(paths, v, i);
    for(j = 0; j < v.nbn; j++)
      free_stack(paths[j]);
  } */
  init_SDL(v);
  callback(v);
  return 0;
}