#ifndef SDL_DRIVER_H_
#define SDL_DRIVER_H_

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "list.h"
#include "fstack.h"
#include "dijkstra.h"

typedef struct input input_t;
struct input {
    int key;
    int mbx, mby;
    int mmx, mmy;
    char quit;
};


enum layout_e {
    VERT_E,
    GRAPH_E,
    PATHS_E,
    LAYOUTS_E
};

enum text_e {
    TVER_E,
    TGRAPH_E,
    TPATHS_E,
    TEXTS_E
};

static int load_texts(vec_t v);
int init_SDL(vec_t v);
void draw_circle(SDL_Point center, int radius, SDL_Color color);
void quit(void);
int element_clicked(int x0, int y0, int x1, int y1);
int element_targeted(int x0, int y0, int x1, int y1);
void update_events(void);
void callback(vec_t v);
static void update(void);
void draw(vec_t v, fstack_t ** paths);
void clear(void);
static void select_vertices(void);
void init_graph_coord(vec_t v);
void draw_edge(vec_t v);

#endif