#ifndef LIST_H_
#define LIST_H_

#define VAL 5

/** \brief Structure représentant la liste de successeurs */
typedef struct list list_t;
struct list {
  unsigned int v;
  struct node * an;   /*!< Adresse du noeud */
  struct list * succ;   /*!< Pointeur vers le successeur suivant */
};

/** \brief Structure représentant un noeud */
typedef struct node node_t;
struct node {
  int in;              /*!< Indice du noeud */
  struct list * l;      /*!< Liste des successeurs */
};

/** \brief Structure représentant un vecteur de tous les noeuds */
typedef struct vec vec_t;
struct vec {
  struct node * n;    /*!< Ensemble des noeuds */
  int nbn;             /*!< Nombre de noeuds */
};

vec_t       create_list(int nbn);
void        generate_list(vec_t v);
void        add_succ(list_t ** l, node_t * adrn, int v);
void        print_list(vec_t v);
void        free_list(vec_t v);
int         my_rand(int min, int max);


#endif