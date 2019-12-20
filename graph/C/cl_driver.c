#include "graph.h"
#include "driver.h"
#include "fstack.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

 #define MAX(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a > _b ? _a : _b; })

 #define MIN(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a < _b ? _a : _b; })

static int   init(const graph_t* graph);
static void  start(void(*callback)(void*));
static void  keydown(void);
static void  draw(fstack_t ** paths, int dep);
static void  quit(void);

driver_t cl_driver = {
    .graph = NULL,
    .in = NULL,
    .init = init,
    .keydown = keydown,
    .start = start,
    .draw = draw,
    .quit = quit
};

#define GRAPH (cl_driver.graph)
#define IN    (cl_driver.in)

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

static void init_title(void) {
    print_title_cl();
    print_graph_cl(GRAPH->vec);
}

static int init(const graph_t* graph) {
    GRAPH = graph;
    cl_driver.in = malloc(sizeof *cl_driver.in);
    assert(cl_driver.in);
    cl_driver.in->quit = 0;
    init_title();
    return 0;
}

static void start(void(*callback)(void*)) {
    for(;;) {
        callback(&cl_driver);
    }
    // libère la mémoire
}

/**
 * \brief Retourne un nombre correspondant à l'indice
 * d'un vertex.
 *
 * \param v vecteur de liste de successeurs (graphe)
 * \return indice d'un vertex;
 */
static void print_choice(void) {
  printf("Choisissez un vertex (0-%d) \n", GRAPH->vec.nbn - 1);
  printf("Voulez-vous quitter (q|Q) ? \n");
  printf("Entrez une commande: ");
}

static void keydown(void) {
  print_choice();
  char * s = (char *)malloc(10 * sizeof(*s));
  assert(s);
  scanf("%s", s);
  normalize(s);
  cl_driver.in->key = strdup(s);
  if(!strcmp(cl_driver.in->key, "q"))
    cl_driver.in->quit = 1;
  cl_driver.in->mbx = cl_driver.in->mby = -1; 
  cl_driver.in->vertex = MIN(GRAPH->vec.nbn - 1, (MAX(0, atoi(cl_driver.in->key))));
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
static void print_pcc(fstack_t ** paths, int dep) {
  int i;
  for(i = 0; i < GRAPH->vec.nbn; i++) {
    printf("start: %s to %s\n", GRAPH->vec.n[dep].name, GRAPH->vec.n[paths[i]->index].name);
    printf("cost:  %d\n", paths[i]->dist);
    if(i != dep) {
      while(!empty_stack(paths[i])) {
        printf("%s -> ", GRAPH->vec.n[pop_stack(paths[i])].name);
      }
    }
    printf("FIN \n\n\n");
  }
}

static void draw(fstack_t ** paths, int dep) {
    print_pcc(paths, dep);
}

static void quit(void) {
    exit(0);
}
