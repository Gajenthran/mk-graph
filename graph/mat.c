#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <time.h>
#include "mat.h"

/**
 * \brief Créer le graphe : matrice compacte en s'aidant
 * de la structure data_t qui contient l'ensemble des données.
 *
 * \param data l'ensemble des données
 * \param n le nombre de données
 * 
 * \return structure mat_t qui correspond à la matrice compacte créée.
 */
mat_t generate_mat(data_t * data, int n) {
  int i, j, k, nel = 0;
  mat_t m;

  m.nba = n * n;
  m.nbn = n;
  m.c = (cell_t *)calloc(0, m.nba * sizeof(*m.c));
  assert(m.c);

  for(i = 0; i < n; i++) {
    for(j = 0; j < data[i].nbn; j++) {
      for(k = 0; k < n; k++) {
        if(!strcmp(data[k].room, data[i].neigh[j])) {
          m.c[nel].n = i;
          m.c[nel].s = k;
          m.c[nel].val = data[i].v[j];
          nel++;
        }
      }
    }
  }

  m.c = (cell_t *)realloc(m.c, nel * sizeof(*m.c));
  assert(m.c);
  m.nba = nel;
  return m;
}

/**
 * \brief Affiche la matrice compacte grâce au données data
 *
 * \param data l'ensemble des données
 * \param n le nombre de données
 */
void print_mat(mat_t m, data_t * data) {
  int i = 0;
  while (i < m.nba) {
    printf("%s: ", data[m.c[i].n].room);
    do {
      printf("%s(%d), ", data[m.c[i].s].room, 
                             m.c[i].val);
      i++;
    } while(i < m.nba && m.c[i].n == m.c[i-1].n);
    printf("\n");
  }
}

/**
 * \brief Libérer la structure représentant la matrice compacte.
 *
 * \param m matrice compacte (le graphe)
 */
void free_mat(mat_t m) {
  if(m.c) {
    free(m.c);
    m.c = NULL;
    m.nbn = 0;
  }
}
