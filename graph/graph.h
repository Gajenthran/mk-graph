/*!
 * \file graphe.h
 * \brief Header de graphe.c
 * \author PANCHALINGAMOORTHY Gajenthran
 * \date 2 Décembre 2020
 */
#ifndef _GRAPH_H_
#define _GRAPH_H_

#include "list.h"
#include "mat.h"
#include "fstack.h"

typedef struct graph graph_t;
typedef struct driver driver_t;

/** \brief Structure représentant le graphe */
struct graph {
  vec_t vec;      // vecteur de liste de sucesseurs
  mat_t mat;      // matrice compacte
  data_t * data;  // données de la bd
  int model;      // choix du modèle (vec, mat)
  int nbn;        // nombre de noeuds
};

enum mode_e { MAT_E, LIS_E, NB_MODELS };

void init_graph(const driver_t*, char *, int);

#endif