/**
 * Pointeur de fonctions pour SDL
 * français/anglais ?
 * static/extern ?
 * pcc
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>
#include "file.h"

#define VAL 5
#define SZ 8

typedef struct noeud noeud_t;
typedef struct vec vec_t;
typedef struct lis lis_t;

/** \brief Structure représentant la liste de successeurs */
typedef struct lis lis_t;
struct lis {
  unsigned int v;
  struct noeud * an;   /*!< Adresse du noeud */
  struct lis * succ;   /*!< Pointeur vers le successeur suivant */
};

/** \brief Structure représentant un noeud */
typedef struct noeud noeud_t;
struct noeud {
  int in;              /*!< Indice du noeud */
  struct lis * l;      /*!< Liste des successeurs */
};

/** \brief Structure représentant un vecteur de tous les noeuds */
typedef struct vec vec_t;
struct vec {
  struct noeud * n;    /*!< Ensemble des noeuds */
  int nbn;             /*!< Nombre de noeuds */
};

vec_t  creer_lis(int nbn);
void   generer_lis_rand(vec_t v);
void   ajouter_succ(lis_t ** l, noeud_t * adrn, int v);
int    dijkstra(vec_t v, int dep, int arr);
void   dijkstra_r(vec_t v, int dep);
void   afficher_lis(vec_t v);
void   liberer_lis(vec_t v);
int *  calculer_nb_succ(vec_t v);
int    my_rand(int min, int max);

int main(void) {
  vec_t v = creer_lis(6);
  // generer_lis_rand(v);
  ajouter_succ(&v.n[0].l, &v.n[2], 3);
  ajouter_succ(&v.n[0].l, &v.n[1], 1);

  ajouter_succ(&v.n[1].l, &v.n[2], 1);
  ajouter_succ(&v.n[1].l, &v.n[3], 8);

  ajouter_succ(&v.n[2].l, &v.n[4], 2);
  ajouter_succ(&v.n[2].l, &v.n[3], 3);
  ajouter_succ(&v.n[2].l, &v.n[5], 2);

  ajouter_succ(&v.n[4].l, &v.n[3], 2);
  ajouter_succ(&v.n[4].l, &v.n[4], 2);

  ajouter_succ(&v.n[5].l, &v.n[4], 1);

  // printf("dij: %d\n", dijkstra(v, 0, 3));
  dijkstra_r(v, 0);
  
  // afficher_lis(v);
  liberer_lis(v);
  return 0;
}

/** \brief Fonction retournant un nombre aléatoire entre deux
 * intervalles.
 *
 * \param min intervalle minimal
 * \param max intervalle maximal
 * 
 * \return Nombre aléatoire entre les intervalles min-max.
 */
int my_rand(int min, int max) {
  return (rand()/(double)RAND_MAX) * (max - min) + min;
}

void dijkstra_r(vec_t v, int dep) {
  int ** cost = (int **)malloc(v.nbn * sizeof(*cost));
  assert(cost);
  int * nbs = calculer_nb_succ(v), dist[v.nbn], pred[v.nbn];
  int dejavu[v.nbn], c, min_d, next_n, i, j;
  for(i = 0; i < v.nbn; i++) {
    cost[i] = (int *)malloc(v.nbn * sizeof(*cost[i]));
    assert(cost[i]);
    for(j = 0; j < v.nbn; j++) {
      cost[i][j] = 1000;
    }
  }

  for(i = 0; i < v.nbn; i++) {
    lis_t * l = v.n[i].l;
    j = 0;
    while(l) {
      // printf("[%d][%d]\n", i, j);
      cost[i][l->an->in] = l->v;
      j++;
      l = l->succ;
    }
  }

  for(i = 0; i < v.nbn; i++) {
    for(j = 0; j < v.nbn; j++) {
      printf("- %d\n", cost[i][j]);
    }
    printf("\n");
  }

  for(i = 0; i < v.nbn; i++) {
    dist[i] = cost[dep][i];
    pred[i] = dep;
    dejavu[i] = 0;
  }

  dist[dep] = 0;
  dejavu[dep] = 1;
  c = 1;

  while(c < v.nbn - 1) {
    min_d = 9999;

    for(i = 0; i < v.nbn; i++) {
      if(dist[i] < min_d && !dejavu[i]) {
        min_d = dist[i];
        next_n = i;
        printf("toto\n");
      }
    }

    dejavu[next_n] = 1;
    for(i = 0; i < v.nbn; i++) {
      if(!dejavu[i]) {
        if(min_d + cost[next_n][i] < dist[i]) {
          dist[i] = min_d + cost[next_n][i];
          pred[i] = next_n;
        }
      }
    }
    printf("\n");
    c++;
  }

  for(i = 0; i < v.nbn; i++) {
    if(i != dep) {
      printf("\nDistance of node %d = %d", i, dist[i]);
      printf("\nPath = %d",i);

      j = i;
      do {
        j = pred[j];
        printf(" <- %d ", j);
      } while(j != dep);
    }
  }
  printf("\n");
}

/**
 * \brief Creer le graphe : liste des successeurs en indiquant 
 * le nombre de noeuds.
 *
 * \param nbn nombre de noeuds
 * 
 * \return structure vec_t qui correspond à l'ensemble des vecteurs 
 * contenant les noeuds du graphe
 */
vec_t creer_lis(int nbn) {
  vec_t vec;
  vec.n = (noeud_t *)malloc(nbn * sizeof(*vec.n));
  assert(vec.n);
  vec.nbn = nbn;

  int i;
  for(i = 0; i < vec.nbn; i++) {
    vec.n[i].in = i;
    vec.n[i].l = (lis_t *)malloc(sizeof(*vec.n[i].l));
    vec.n[i].l = NULL;
  }
  return vec; 
}

/**
 * \brief Ajouter un successeur à la liste de successeurs.
 *
 * \param l    liste de successeurs
 * \param adrn adresse du noeud successeur
 */
void ajouter_succ(lis_t ** l, noeud_t * adrn, int v) {
  while(*l) {
    if((*l)->an == adrn)
      return;
    l = &((*l)->succ);
  }

  lis_t * s = (lis_t *)malloc(sizeof(*s));
  assert(s);
  s->an = (noeud_t *)malloc(sizeof(*s->an));
  assert(s->an);
  s->an = adrn;
  s->v = v;
  s->succ = NULL; 

  s->succ = *l;
  (*l) = s;
}

/**
 * \brief Afficher les noeuds du graphe (Lis) où chaque valeur
 * d'un noeud correspond à l'indice des fonctions de fpr.
 *
 * \param v   vecteur de noeuds (le graphe)
 */
void afficher_lis(vec_t v) {
  int i;
  lis_t * l;
  for(i = 0; i < v.nbn; i++) {
    printf("%d\n", v.n[i].in);
    l = v.n[i].l;
    while(l) {
      printf(" -> %d (%d)\n", l->an->in, l->v);
      l = l->succ;
    }
  }
}

/**
 * \brief Libérer la structure représentant la liste de successeurs.
 *
 * \param v vecteur de noeuds (le graphe)
 */
void liberer_lis(vec_t v) {
  int i;
  lis_t * tmp;
  for(i = 0; i < v.nbn; i++) {
    while(v.n[i].l) {
      tmp = v.n[i].l;
      v.n[i].l = v.n[i].l->succ;
      free(tmp);
    }
  }
}

void generer_lis_rand(vec_t v) {
  srand(time(NULL));
  int i, ri, rv, rsucc;
  for(i = 0; i < v.nbn; i++) {
    rsucc = rand() % 4;
    while(rsucc) {
      ri = rand() % v.nbn;
      rv = my_rand(1, VAL);
      ajouter_succ(&v.n[i].l, &v.n[ri], rv);
      rsucc--;
    }
  }
}

int * calculer_nb_succ(vec_t v) {
  int s, i, * nb_succ = (int *)malloc(v.nbn * sizeof(*nb_succ));
  assert(nb_succ);

  for(i = 0; i < v.nbn; i++) {
    s = 0;
    lis_t * l = v.n[i].l;
    while(l) { l = l->succ; s++; }
    nb_succ[i] = s;
  }

  return nb_succ;
}