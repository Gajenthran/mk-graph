#ifndef MAT_H_
#define MAT_H_

#include "parser.h"

typedef struct mat mat_t;
typedef struct cell cell_t;

/** \brief Structure représentant les cellules de la matrice compacte */
struct cell {
  int n;              /*!< Indice du noeud */
  int s;              /*!< Indice du successeur */
  int val;            /*!< Indice du successeur */
};

/** \brief Structure représentant la matrice compacte */
struct mat {
  struct cell * c;    /*!< L'ensemble des cellules de la mco de taille nba */
  int nba;            /*!< Nombre d'arêtes */
  int nbn;            /*!< Nombre de noeuds */
};

mat_t generate_mat(data_t * data, int n);
void free_mat(mat_t m);
void print_mat(mat_t m, data_t * data);

#endif