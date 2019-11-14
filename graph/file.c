/*!
 * \file queue.c
 * \brief a FIFO (first-in-first-out) data structure
 * \author PANCHALINGAMOORTHY Gajenthran. Adapted from Farès Belhadj's course (code).
 * \date 4 August 2018
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "file.h"

extern file_t * init_f(void) {
	file_t * f = (file_t *)malloc(sizeof(*f));
	assert(f);
	f->size = SIZE_F;
	f->n = (node_t *)malloc(f->size * sizeof(*f->n));
	assert(f->n);
	f->top = f->bottom = 0;
	return f;
}

extern void enfiler_f(file_t * f, int dist, int index) {
	node_t n = {dist, index};
	f->n[f->top++] = n;
	if(f->top > f->size-1) {
		f->n = realloc(f->n, (f->size *= 2) * sizeof(*f->n));
		assert(f->n);
	}
}

void enfiler_der_f(file_t * f, int dist, int index) {
	int i, t = f->top;
	node_t n = {dist, index};
	for(i = 0; i < t; i++)
		f->n[i + 1] = f->n[i];
	f->n[0] = n;
	f->top++;
}

extern int get_taille(file_t * f) {
	return f->top;
}

extern node_t defiler_f(file_t * f) {
	node_t n = f->n[f->bottom++];
	if(f->bottom > f->size)
		f->bottom = 0;
	return n; 
}

node_t defiler_der_f(file_t * f) {
	node_t n = f->n[--f->top];
	return n;
}

extern int vide_f(file_t * f) {
	return f->bottom == f->top;
}

/*! \brief clean the queue. */
extern void liberer_f(file_t * f) {
	if(f)
		free(f);
	f->top = f->bottom = 0;
}