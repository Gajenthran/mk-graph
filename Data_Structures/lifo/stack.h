#ifndef STACK_H___
#define STACK_H___

#define SIZE_STACK 1024 

void 	initStack(void);
void	push(int val);
int		pop(void);
int		empty(void);
void	clean(void);

#endif