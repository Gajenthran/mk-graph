#ifndef FSTACK_H_
#define FSTACK_H_

#define SIZE_F 1024

typedef struct stack fstack_t;
struct stack {
  int * paths;
  int size, top;
  int index, dist;
};

extern fstack_t * init_stack(int index, int dist);
extern void      push_stack(fstack_t * q, int p);
extern int       pop_stack(fstack_t * q);
extern int       empty_stack(fstack_t * q);
extern void      free_stack(fstack_t * q);

#endif