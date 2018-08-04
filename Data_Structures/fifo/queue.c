#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "fifo.h"

static int * queue = NULL;
static int size;
static int bottom = 0, top = 0;

extern void initQueue(void) {
	if(queue) {
		size *= 2;
		queue = realloc(queue, size);
	} 

	else {
		size = SIZE_QUEUE;
		queue = malloc(size * sizeof(*queue));
		assert(queue);
		top = bottom = 0;
		atexit(clean);
	}
}

extern void enqueue(int v) {
	queue[top++] = v;
	if(top > size-1)
		initQueue();
}

extern int dequeue(void) {
	int v = queue[bottom++];
	if(bottom > size)
		bottom = 0;

	return v;
}

extern int empty(void) {
	return bottom == top;
}

extern void clean(void) {
	if(queue)
		free(queue);
	top = bottom = 0;
}