/*!
 * \file driver.h
 * \brief Header permettant de faire la transition
 * entre les deux drivers (sdl, cl).
 * \author PANCHALINGAMOORTHY Gajenthran
 * \date 2 Décembre 2020
 */
typedef struct driver driver_t;
typedef struct input input_t;

/** \brief Structure représentant les entrées
 * utilisateurs. */
struct input {
  int vertex;   // vertex choisi
  int paths;    // chemin choisi
  char *key;    // entrée clavier
  int mbx, mby; // coordonnées de la souris cliquée
  int mmx, mmy; // coordonnées de la position de la souris
  char quit;    // booléen pour vérifier l'arrêt du programme
};

struct driver {
  const graph_t* graph;
  input_t * in;
  int  (*init)(const graph_t* graph);
  void (*start)(void(*callback)(void*));
  void (*keydown)(void);
  void (*draw)(fstack_t ** paths, int dep);
  void (*quit)(void);
};