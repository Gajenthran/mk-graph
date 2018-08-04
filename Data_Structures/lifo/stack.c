#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "stack.h"

static int * stack = NULL;
static int size = 0;
static int top = -1;

void initStack(void) {
	if(stack) {
		size *= 2;
		stack = realloc(stack, size);
	} 

	else {
		size = SIZE_STACK;
		stack = malloc(size * sizeof(*stack));
		assert(stack);
		top = -1;
		atexit(clean);
	}
}

void push(int val) {
	if(top > size-1)
		initStack();

	stack[++top] = val;
}

int pop(void) {
	return stack[top--];
}

int empty(void) {
	return top < 0;
}

void clean(void) {
	if(stack)
		free(stack);
	top = -1;
}