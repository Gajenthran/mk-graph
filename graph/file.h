#ifndef FILE_H_
#define FILE_H_

#define SIZE_F 1024

typedef struct node node_t;
struct node {
  int dist, index;
};

typedef struct file file_t;
struct file {
  node_t * n;
  int size, bottom, top;
};

extern file_t * init_f(void);
extern void     enfiler_f(file_t * f, int dist, int index);
extern void     enfiler_der_f(file_t * f, int dist, int index);
extern node_t   defiler_f(file_t * f);
extern node_t   defiler_der_f(file_t * f);
extern int      vide_f(file_t * f);
extern void     liberer_f(file_t * f);
extern int      get_taille(file_t * f);

#endif