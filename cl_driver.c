/*!
 * \file cl_driver.c
 * \brief Un driver CL (command line) 
 * pour le graphe.
 * \author PANCHALINGAMOORTHY Gajenthran
 * \date 2 Décembre 2020
 */
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

static void  _init_title(void);
static void  _init_config(void);
static void  _print_title_cl(void);
static void  _print_graph_cl(void);
static void  _print_choice(void);
static void  _print_pcc(fstack_t ** paths, int dep);

static int   init(const graph_t* graph);
static void  start(void(*callback)(void*));
static void  keydown(void);
static void  draw(fstack_t ** paths, int dep);
static void  quit(void);

driver_t _cl_driver = {
	.graph = NULL,
	.in = NULL,
	.init = init,
	.keydown = keydown,
	.start = start,
	.draw = draw,
	.quit = quit
};

#define GRAPH (_cl_driver.graph)
#define IN    (_cl_driver.in)

/** 
 * \brief Initialise les paramètres du graphe et SDL.
 * \param graph la structure représentant le graphe
 *
 * \return 0 si tout s'est déroulé correctement, 1 sinon.
 */
static int init(const graph_t* graph) {
	GRAPH = graph;
	_init_config();
	_init_title();
	return 0;
}

/** \brief Récupère les entrées utilisateurs. */
static void keydown(void) {
	_print_choice();
	char * s = (char *)malloc(10 * sizeof(*s));
	assert(s);
	scanf("%s", s);
	normalize(s);
	IN->key = strdup(s);
	if(!strcmp(IN->key, "q"))
		IN->quit = 1;
	IN->mbx = IN->mby = -1; 
	IN->vertex = MIN(GRAPH->nbn - 1, (MAX(0, atoi(IN->key))));
}

/** 
 * \brief Fonction de callback qui lance le
 * programme. 
 */
static void start(void(*callback)(void*)) {
	for(;;) {
		callback(&_cl_driver);
	}
}

/** 
 * \brief Dessine les différentes parties du programme à
 * savoir les différentes vertices présents dans le graphe,
 * le graphe et les chemins.
 *
 * \param graph la structure représentant le graphe
 * \param dep point de départ du vertex
 */
static void draw(fstack_t ** paths, int dep) {
	_print_pcc(paths, dep);
}

/** \brief Quitte le programme. */
static void quit(void) {
	exit(0);
}

/** 
 * \brief Initialise les paramètres de configuration 
 * (entrées, graphe)
 */
static void _init_config(void) {
	IN = malloc(sizeof *IN);
	assert(IN);
	IN->quit = 0;
}

 /** \brief Dessine le titre du programme. */
static void _init_title(void) {
	_print_title_cl();
	_print_graph_cl();
}

/**
 * \brief Affiche le titre et explique le cas
 * d'utilisation dans lequel est utilisé l'algorithme
 * de Dijkstra.
 */
static void _print_title_cl(void) {
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
static void _print_graph_cl(void) {
	printf("Voici le graphe représentant le labyrinthe (sous la forme \n"
			   "de liste de successeurs)\n");
	if(GRAPH->model == LIS_E) {
		print_name_list(GRAPH->vec);
	} else {
		print_mat(GRAPH->mat, GRAPH->data);
	}
	printf("\n\n");
}

/**
 * \brief Retourne un nombre correspondant à l'indice
 * d'un vertex.
 *
 * \param v vecteur de liste de successeurs (graphe)
 * \return indice d'un vertex;
 */
static void _print_choice(void) {
	printf("Choisissez un vertex (0-%d) \n", GRAPH->nbn - 1);
	printf("Voulez-vous quitter (q|Q) ? \n");
	printf("Entrez une commande: ");
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
static void _print_pcc(fstack_t ** paths, int dep) {
	int i;
	for(i = 0; i < GRAPH->nbn; i++) {
		printf("start: %s to %s\n", GRAPH->data[dep].name, GRAPH->data[paths[i]->index].name);
		printf("cost:  %d\n", paths[i]->dist);
		if(i != dep) {
			while(!empty_stack(paths[i])) {
				printf("%s -> ", GRAPH->data[pop_stack(paths[i])].name);
			}
		}
		printf("FIN \n\n\n");
	}
}
