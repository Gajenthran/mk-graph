#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "cl_driver.h"

static char * input_cl(void);
static int    choose_vertex_cl(vec_t v);
static int    quit_cl(void);
static void   print_title_cl(void);
static void   print_graph_cl(vec_t v);
static void   print_pcc_cl(fstack_t ** paths, vec_t v, int dep);

/**
 * \brief Initialise le contexte de la ligne
 * de commande.
 *
 * \param v vecteur de liste de successeurs (graphe)
 */
void init_cl(vec_t v) {
  print_title_cl();
  print_graph_cl(v);
}

/**
 * \brief Fonction de callback qui permet d'utiliser
 * l'algorithme de Dijkstra pour chercher le plus
 * court chemin.
 *
 * \param v vecteur de liste de successeurs (graphe)
 */
void callback_cl(vec_t v) {
  int vertex, done = 0;
  fstack_t ** paths = NULL;
  while(!done) {
    vertex = choose_vertex_cl(v);
    paths = dijkstra(v, vertex);
    print_pcc_cl(paths, v, vertex);
    done = quit_cl();
  }
}

/**
 * \brief Retourne les entrées utilisateur.
 *
 * \return les entrées utilisateur
 */
static char * input_cl(void) {
  char * s = (char *)malloc(10 * sizeof(*s));
  assert(s);
  scanf("%s", s);
  normalize(s);
  return s;
}

/**
 * \brief Retourne un nombre correspondant à l'indice
 * d'un vertex.
 *
 * \param v vecteur de liste de successeurs (graphe)
 * \return indice d'un vertex;
 */
static int choose_vertex_cl(vec_t v) {
  printf("Choisissez un vertex (0-%d): ", v.nbn);
  char * s = input_cl();
  return atoi(s);
}

/**
 * \brief Vérifie si l'utilisateur souhaite quitter
 * le programme.
 *
 * \return 1 si l'utilisateur souhaite quitter, 0 sinon.
 */
static int quit_cl(void) {
  printf("Voulez-vous quitter (q|Q) : ");
  char * s = input_cl();
  return !strcmp(s, "q");
}


/**
 * \brief Affiche le titre et explique le cas
 * d'utilisation dans lequel est utilisé l'algorithme
 * de Dijkstra.
 */
static void print_title_cl(void) {
  printf("| PCC Dijkstra |\n");
  printf("Voici la modélisation d'un labyrinthe à travers un graphe orienté.\n"
        "Chaque salle donne accès (ou non) à une autre salle, il est parfois \n"
        "impossible de retourner dans la salle précédente. Chaque déplacement \n"
        "vers une salle consomme de l'énergie. Le but est de consommer le \n"
        "moins d'énergie possible pour passer d'une salle à une autre.\n\n");
}

/**
 * \brief Affiche l'ensemble du graphe sous la forme de 
 * liste de successeurs. Les flèches représentants 
 * les successeurs d'un vertex.
 *
 * \param v vecteur de liste de successeurs (graphe)
 */
static void print_graph_cl(vec_t v) {
  printf("Voici le graphe représentant le labyrinthe (sous la forme \n"
       "de liste de successeurs)\n");
  print_name_list(v);
  printf("\n\n");
}

/**
 * \brief Affiche les plus courts chemins d'un vertex données par 
 * rapport aux autres vertex, en affichant également le 
 * cout de chaque chemin.
 *
 * \param paths liste des chemins pour un vertex donné
 * \param v     vecteur de liste de successeurs (graphe)
 * \param dep   vertex de départ
 */
static void print_pcc_cl(fstack_t ** paths, vec_t v, int dep) {
  int i;
  for(i = 0; i < v.nbn; i++) {
    printf("start: %s to %s\n", v.n[dep].name, v.n[paths[i]->index].name);
    printf("cost:  %d\n", paths[i]->dist);
    if(i != dep) {
      while(!empty_stack(paths[i])) {
        printf("%s -> ", v.n[pop_stack(paths[i])].name);
      }
    }
    printf("FIN \n\n\n");
  }
}