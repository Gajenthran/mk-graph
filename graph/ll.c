#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "ll.h"

list_t * init_list(void) {
  list_t * l = (list_t *)malloc(sizeof *l);
  assert(l);
  l->head = NULL;
  l->size = 0;
  return l;
}

void insert_list(list_t * l, int index) {
  lnode_t * n = (lnode_t *)malloc(sizeof *n);
  n->index = index;
  n->next = l->head;
  l->head = n;
  l->size++;
}

void print_list(list_t * l) {
  if(!l || !l->head)
    return;

  lnode_t * h = l->head;
  while(h) {
    printf("%d -> ", h->index);
    h = h->next;
  }
  printf("NULL\n");
}

void free_list(list_t * l) {
  if(!l)
    return;

  while(l->head) {
    lnode_t * n = l->head;
    l->head = l->head->next;
    free(n);
  }
  l->size = 0;
}

void remove_list(list_t * l) {
  if(!l)
    return;
  lnode_t * n = l->head;
  l->head = l->head->next;
  free(n);
  l->size--;
}

/* int main(void) {
  list_t * l = init_list();
  insert_list(l, 10);
  insert_list(l, 12);
  insert_last_list(l, 20);
  print_list(l);
  free_list(l);
  return 0;
} */