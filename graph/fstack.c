/*!
 * \file fstack.c
 * \brief Représentation d'une pile (FIFO)
 * \author PANCHALINGAMOORTHY Gajenthran
 * \date 2 Décembre 2020
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "fstack.h"

/**
 * Initialise la pile de données qui contiendra
 * la liste des indices de vertex ainsi que la 
 * distance.
 *
 * \param index indice du vertex de départ
 * \param dist  distance du vertex
 */
fstack_t * init_stack(int index, int dist) {
  fstack_t * q = (fstack_t *)malloc(sizeof(*q));
  assert(q);
  q->size = SIZE_F;
  q->index = index;
  q->dist = dist;
  q->nel = 0;
  q->paths = (int *)malloc(q->size * sizeof(*q->paths));
  assert(q->paths);
  q->top = -1;
  return q;
}

/**
 * Empile une donnée à savoir un vertex.
 *
 * \param q pile de données
 * \param p indice du vertex 
 */
void push_stack(fstack_t * q, int p) {
  if(q->top > q->size-1) {
    q->paths = realloc(q->paths, (q->size *= 2) * sizeof(*q->paths));
    assert(q->paths);
  }
  q->paths[++q->top] = p;
  if(q->top >= q->nel) q->nel++;
}

/**
 * Dépiler une donnée à savoir un vertex.
 *
 * \param q pile de données
 * \return indice du vertex 
 */
int pop_stack(fstack_t * q) {
  return q->paths[q->top--];
}

/**
 * Vérifie si la pile est vide.
 *
 * \param q pile de données
 * \return 1 si la pile est vide, 0 sinon.
 */
int empty_stack(fstack_t * q) {
  return q->top < 0;
}

/**
 * \brief Rempiler les données qui ont été dépilés.
 *
 * \param q pile de données
 */
void rewind_stack(fstack_t * q) {
  q->top = q->nel-1;
}

/*! \brief Libère la mémoire de la pile. */
void free_stack(fstack_t * q) {
  if(q)
    free(q);
  q->top = -1;
}