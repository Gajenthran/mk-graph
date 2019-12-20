typedef struct driver driver_t;
typedef struct input input_t;

struct input {
  int vertex;
  int paths;
  char *key;
  int mbx, mby;
  char quit;
};

struct driver {
  int val;
  const graph_t* graph;
  input_t * in;
  int  (*init)(const graph_t* graph);
  void (*start)(void(*callback)(void*));
  void (*keydown)(void);
  void (*draw)(fstack_t ** paths, int dep);
  void (*quit)(void);
};

enum dr_e {
  CL_E, SDL_E, NB_DR
};