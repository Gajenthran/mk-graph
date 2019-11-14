#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>
#include "list.h"

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

/**
 * \brief Creer le graphe : liste des successeurs en indiquant 
 * le nombre de noeuds.
 *
 * \param nbn nombre de noeuds
 * 
 * \return structure vec_t qui correspond à l'ensemble des vecteurs 
 * contenant les noeuds du graphe
 */
vec_t create_list(int nbn) {
  vec_t vec;
  vec.n = (node_t *)malloc(nbn * sizeof(*vec.n));
  assert(vec.n);
  vec.nbn = nbn;

  int i;
  for(i = 0; i < vec.nbn; i++) {
    vec.n[i].in = i;
    vec.n[i].l = (list_t *)malloc(sizeof(*vec.n[i].l));
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
void add_succ(list_t ** l, node_t * adrn, int v) {
  while(*l) {
    if((*l)->an == adrn)
      return;
    l = &((*l)->succ);
  }

  list_t * s = (list_t *)malloc(sizeof(*s));
  assert(s);
  s->an = (node_t *)malloc(sizeof(*s->an));
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
void print_list(vec_t v) {
  int i;
  list_t * l;
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
void free_list(vec_t v) {
  int i;
  list_t * tmp;
  for(i = 0; i < v.nbn; i++) {
    while(v.n[i].l) {
      tmp = v.n[i].l;
      v.n[i].l = v.n[i].l->succ;
      free(tmp);
    }
  }
}

void generate_list(vec_t v) {
  srand(time(NULL));
  int i, ri, rv, rsucc;
  for(i = 0; i < v.nbn; i++) {
    rsucc = rand() % 4;
    while(rsucc) {
      ri = rand() % v.nbn;
      rv = my_rand(1, VAL);
      add_succ(&v.n[i].l, &v.n[ri], rv);
      rsucc--;
    }
  }
}