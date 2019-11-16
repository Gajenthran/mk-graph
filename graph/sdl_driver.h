#ifndef SDL_DRIVER_H_
#define SDL_DRIVER_H_

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "list.h"
#include "fstack.h"
#include "dijkstra.h"

/** \brief Structure représentant les entrées utilisateurs */
typedef struct input input_t;
struct input {
    int mbx, mby; // coordonnées du curseur
    char quit;    // booléan pour vérifier les sorties du prog
};

/** Enumération des différents layouts */
enum layout_e {
    VERT_E,    // layout pour les vertices
    GRAPH_E,   // layout pour le graphe
    PATHS_E,   // layout pour les chemins
    LAYOUTS_E 
};

/** Enumération des différents textes */
enum text_e {
    TVER_E,   // texte pour les vertices
    TGRAPH_E, // texte pour le graphe
    TPATHS_E, // texte pour les chemins
    TEXTS_E
};

int init_SDL(vec_t v);
void callback(vec_t v);

#endif