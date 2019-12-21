/*!
 * \file list.h
 * \brief Header de list.c
 * \author PANCHALINGAMOORTHY Gajenthran
 * \date 2 Décembre 2020
 */
#ifndef LIST_H_
#define LIST_H_

#include "parser.h"

/** \brief Structure représentant la liste de successeurs */
typedef struct list list_t;
struct list {
  unsigned int v;
  struct node * an;   // adresse du noeud 
  struct list * succ; // Pointeur vers le successeur suivant
};

/** \brief Structure représentant un noeud */
typedef struct node node_t;
struct node {
  char * name;     // nom du noeud 
  int in;          // indice du noeud
  struct list * l; // liste des successeurs
};

/** \brief Structure représentant un vecteur de tous les noeuds */
typedef struct vec vec_t;
struct vec {
  struct node * n; // ensemble des noeuds
  int nbn;         // nombre de noeuds
};

vec_t generate_list(data_t * data, int n);
void  add_succ(list_t ** l, node_t * adrn, int v);
void  print_list(vec_t v);
void  print_name_list(vec_t v);
void  free_list(vec_t v);


#endif