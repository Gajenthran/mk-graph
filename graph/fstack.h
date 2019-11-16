#ifndef FSTACK_H_
#define FSTACK_H_

#define SIZE_F 1024

/** \brief Structure représentant une pile de données */
typedef struct stack fstack_t;
struct stack {
  int * paths; // vecteur de chemins
  int size, top; // taille et indice actuel du vecteur
  int index, dist, nel; // indice, distance du vertex
};

fstack_t * init_stack(int index, int dist);
void       push_stack(fstack_t * q, int p);
int        pop_stack(fstack_t * q);
int        empty_stack(fstack_t * q);
void       rewind_stack(fstack_t * q);
void       free_stack(fstack_t * q);

#endif