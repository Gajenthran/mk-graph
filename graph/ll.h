#ifndef LL_H_
#define LL_H_

typedef struct lnode lnode_t;
struct lnode {
  int index;
  lnode_t * next;
};

typedef struct list list_t;
struct list {
  lnode_t * head;
  int size;
};

extern list_t * init_list(void);
extern void insert_list(list_t * l, int index);
extern void print_list(list_t * l);
extern void free_list(list_t * l);

#endif