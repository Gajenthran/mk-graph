/**
 * remove the infinity
 * début des fichiers (auteur)
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
#include "cl_driver.h"

/**
 * \brief Usage que doit adopter l'utilisateur pour
 * lancer le programme.
 */
void usage(char * prog) {
  fprintf(stderr, "Usage: %s <file> [-sdl]", prog);
  exit(0);
}

int main(int argc, char *argv[]) {
  int n;
  char * filename = NULL;
  // vérifie le nom du fichier
  if(argc >= 2) {
    filename = read_file(argv[1]);
  } else {
    filename = read_file("files/maze.txt");
  }

  // normalise le texte pour l'exploiter
  normalize(filename);
  data_t * data = tokenize(filename, &n);

  // transforme en liste de successeurs 
  vec_t v = generate_list(data, n);

  // évènement + affichage en CLI et GUI
  if(argc >= 3) {
    if(!strcmp("-sdl", argv[2])) {
      init_SDL(v);
      callback(v);
    } else {
      usage(argv[0]);
    }
  } else {
    init_cl(v);
    callback_cl(v);
  }

  return 0;
}