/*!
 * \file stack.c
 * \brief a FIFO (first-in-first-out) data structure
 * \author PANCHALINGAMOORTHY Gajenthran. Adapted from Farès Belhadj's course (code).
 * \date 4 August 2018
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "fstack.h"

extern fstack_t * init_stack(int index, int dist) {
  fstack_t * q = (fstack_t *)malloc(sizeof(*q));
  assert(q);
  q->size = SIZE_F;
  q->index = index;
  q->dist = dist;
  q->paths = (int *)malloc(q->size * sizeof(*q->paths));
  assert(q->paths);
  q->top = -1;
  return q;
}

extern void push_stack(fstack_t * q, int p) {
  if(q->top > q->size-1) {
    q->paths = realloc(q->paths, (q->size *= 2) * sizeof(*q->paths));
    assert(q->paths);
  }
  q->paths[++q->top] = p;
}

extern int pop_stack(fstack_t * q) {
  return q->paths[q->top--];
}

extern int empty_stack(fstack_t * q) {
  return q->top < 0;
}

/*! \brief clean the stack. */
extern void free_stack(fstack_t * q) {
  if(q)
    free(q);
  q->top = -1;
}