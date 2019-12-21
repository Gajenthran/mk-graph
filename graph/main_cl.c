/*!
 * \file main_cl.c
 * \brief Le fichier main pour le driver cl
 * \author PANCHALINGAMOORTHY Gajenthran
 * \date 2 Décembre 2020
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "graph.h"

extern driver_t _cl_driver;

/** \brief La commande d'usage pour lancer le programme. */
void usage(char * prog) {
  fprintf(stderr, "Usage: ./%s <file> [-mat] [-lis]\n", prog);
  exit(1);
}

int main(int argc, char *argv[]) {
  int i, model = 0;
  char * filename = NULL;
  if(argc == 1) {
    // si rien n'est spécifié, fichier de base
    filename = strdup("files/maze.txt");
  } else if(argc == 2) {
    // fichier précisé
    filename = strdup(argv[1]);
  } else if(argc == 3) {
    // fichier précisé + graphe
    filename = strdup(argv[1]);
    if(!strcmp(argv[2], "-lis")) {
      model = LIS_E;
    } else if(!strcmp(argv[2], "-mat")) {
      model = MAT_E;
    } else {
      usage(argv[0]);
    }
  }
  init_graph(&_cl_driver, filename, model);
  return 0;
}