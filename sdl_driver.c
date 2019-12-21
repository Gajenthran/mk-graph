/*!
 * \file sdl_driver.c
 * \brief Un driver SDL pour le graphe.
 * \author PANCHALINGAMOORTHY Gajenthran
 * \date 2 Décembre 2020
 */
#include <stdio.h>
#include <assert.h>
#include <unistd.h>
#include <time.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "list.h"
#include "fstack.h"
#include "dijkstra.h"
#include "driver.h"

static int   _load_texts(void);
static void  _init_config(void);
static void  _init_graph_coord(void);
static int   _init_SDL(void);
static void  _draw_hl(void);
static void  _draw_circle(SDL_Point, int, SDL_Color);
static void  _draw_title(void);
static void  _draw_vertices(void);
static void  _draw_paths(fstack_t **);
static void  _draw_edge(fstack_t **);
static int   _element_clicked(int, int, int, int);
static int   _element_target(int, int, int, int);
static void  _select_vertices(void);
static void  _select_paths(void);
static int   _select_end(fstack_t ** paths);
static void  _update(void);
static void  _clear(void);
static void  _clean(void);
static void  _clean_tab(int *, int);

static int   init(const graph_t*);
static void  start(void(*callback)(void*));
static void  keydown(void);
static void  draw(fstack_t **, int);
static void  quit(void);

driver_t _sdl_driver = {
  .graph = NULL,
  .in = NULL,
  .init = init,
  .keydown = keydown,
  .start = start,
  .draw = draw,
  .quit = quit
};

#define GRAPH (_sdl_driver.graph)
#define IN    (_sdl_driver.in)

/** Enumération des diffé_rents layouts */
enum layout_e {
  VERT_E,   // layout pour les vertices
  GRAPH_E,  // layout pour le graphe
  PATHS_E,  // layout pour les chemins
  LAYOUTS_E 
};

/** Enumération des diffé_rents textes */
enum text_e {
  TVER_E,    // texte pour les vertices
  TGRAPH_E,  // texte pour le graphe
  TPATHS_E,  // texte pour les chemins
  TEXTS_E
};

/**
 * \brief Textes représentant les titres présent
 * dans la fenêtre SDL.
 */
static const char * _texts[] = {
  "Vertices:", "Graph:", "Paths:"
};

/**
 * \brief Position des textes représentant les titres 
 * présent dans la fenêtre SDL.
 */
static const SDL_Rect _txt_rec[TEXTS_E] = {
  {15, 10, 90, 90},
  {15, 300, 70, 70},
  {930, 300, 70, 70}
};

/**
 * \brief Layouts représentant les diffé_rentes parties
 * du programme dans la fenêtre SDL.
 */
static const SDL_Rect _lay_rec[LAYOUTS_E] = {
  {0, 0, 1280, 280},
  {0, 280, 890, 680},
  {890, 280, 390, 680},
};

/**
 * \brief Position de l'affichage du chemin d'un vertex
 * vers un autre.
 */
static SDL_Rect _tpt_rec = {
  830 + 50, 280, 120, 50
};

/** \brief Position de l'affichage des nom de vertices */
static SDL_Rect v_rec = {30, 90, 70, 70};
/** \brief Les vertices visités */
static int * _vvisited;
/** \brief Position de l'affichage des vertices */
static SDL_Rect * _gr_rec = NULL; 
/** \brief Fenêtre du programme SDL */
static SDL_Window *_win;
/** \brief Rendu du programme SDL */
static SDL_Renderer *_ren;
/** \brief Liste des textures de texte du programme SDL */
static SDL_Texture ** _txtTex;
/** \brief Liste des SDL couleurs sous la forme RGB */
static SDL_Color _black = { 0, 0, 0, 200 },
                 _red   = { 200, 22, 9, 255},
                 _blue  = { 18, 184, 234, 140},
                 _yellow = {200, 180, 41, 100};
/** \brief Police utilisée pour les textes */
static TTF_Font * _font = NULL;
/** \brief Nombre de textes présenté dans le programme */
static int _ntexts = 0;

/** 
 * \brief Initialise les paramètres du graphe et SDL.
 * \param graph la structure représentant le graphe
 *
 * \return 0 si tout s'est déroulé correctement, 1 sinon.
 */
static int init(const graph_t* graph) {
  GRAPH = graph;
  _init_config();
  _init_SDL();
  atexit(_clean);
  return 0;
}

/** \brief Récupère les entrées utilisateurs. */
static void keydown(void) {
  SDL_Event ev;
  srand((unsigned int)time(NULL));
  int t0, t = SDL_GetTicks();
  while(SDL_PollEvent(&ev) != 0) {
    switch(ev.type) {
      case SDL_QUIT:
        IN->quit = 1;
        break;
      case SDL_MOUSEBUTTONDOWN:
        if(ev.button.button == SDL_BUTTON_LEFT) {
          IN->mbx = ev.button.x;
          IN->mby = ev.button.y;
        }
        break;
      case SDL_MOUSEMOTION:
        IN->mmx = ev.motion.x;
        IN->mmy = ev.motion.y;
        break;
      default:
        break;
    }

    t0 = SDL_GetTicks() - t;

    _select_vertices();
    _select_paths();
    if(t0 < 50) {
      SDL_Delay(50 - t0);
    }
  }
}

/** 
 * \brief Fonction de callback qui lance le
 * programme. 
 */
static void start(void(*callback)(void*)) {
    for(;;) {
      callback(&_sdl_driver);
      usleep(135000);
    }
}

/** 
 * \brief Dessine les différentes parties du programme à
 * savoir les différentes vertices présents dans le graphe,
 * le graphe et les chemins.
 *
 * \param graph la structure représentant le graphe
 * \param dep point de départ du vertex
 */
static void draw(fstack_t ** paths, int dep) {
  _clear();
  int i;
  SDL_SetRenderDrawColor(_ren, 0, 0, 0, 255);
  _draw_title();
  _draw_vertices();
  _select_end(paths);
  for(i = 0; i < _ntexts - TEXTS_E; i++) {
    SDL_Point p = {
      _gr_rec[i].x + _gr_rec[i].w/2, 
      _gr_rec[i].y + _gr_rec[i].h/2
    };
    if(_vvisited[i]) _draw_circle(p, 20, _blue);
    else _draw_circle(p, 20, _red);
    SDL_RenderCopy(_ren, _txtTex[i + TEXTS_E], NULL, &_gr_rec[i]);
  }
  SDL_SetRenderDrawColor(_ren, 0, 0, 0, 255);
  _draw_edge(paths);
  _draw_paths(paths);
  _draw_hl();
  _update();
}

/** \brief Quitte le programme. */
static void quit(void) {
  SDL_Quit();
  exit(0);
}

/** 
 * \brief Charger tous les textes, càd transformer tous 
 * les textes en textures. 
 *
 * \param v vecteur de liste de successeurs (graphe)
 * \return 0 si tout s'est déroulé correctement, 1 sinon.
 */
static int _load_texts(void) {
  _txtTex = (SDL_Texture **)malloc((GRAPH->nbn + TEXTS_E) * sizeof(*_txtTex));
  assert(_txtTex);

  _ntexts = GRAPH->nbn + TEXTS_E;
  int i;
  _font = TTF_OpenFont("files/Roboto-Condensed.ttf", 30);
  SDL_Surface *t;
  for(i = 0; i < TEXTS_E + GRAPH->nbn; ++i) {
    if(i < TEXTS_E) {
      t = TTF_RenderText_Solid(_font, _texts[i], _black);
    } else {
      t = TTF_RenderText_Solid(_font, strndup(GRAPH->data[i - TEXTS_E].name, 5), _black);
    }
    if(t == NULL) {
      SDL_DestroyRenderer(_ren);
      SDL_DestroyWindow(_win);
      printf("Error: %s\n", SDL_GetError());
      SDL_Quit();
      return 1;
    }

    _txtTex[i] = SDL_CreateTextureFromSurface(_ren, t);
    SDL_FreeSurface(t);
    t = NULL;
  }
  return 0;
}

/** 
 * \brief Initialise les paramètres de configuration 
 * (entrées, graphe)
 */
static void _init_config(void) {
  IN = malloc(sizeof *IN);
  assert(IN);
  IN->quit = 0;
  IN->paths = -1;
  _vvisited = (int *)calloc(0, GRAPH->nbn * sizeof(*_vvisited));
  assert(_vvisited);
}

/** 
 * \brief Initialise les coordonnées pour
 * l'affichage du graphe.
 *
 * \param v vecteur de liste de successeurs (graphe)
 */
static void _init_graph_coord(void) {
  srand(time(NULL));
  int xr, yr, i;
  _gr_rec = (SDL_Rect *)malloc(GRAPH->nbn * sizeof(*_gr_rec));
  assert(_gr_rec);
  int mid = (_lay_rec[GRAPH_E].w - 50) / 2;
  for(i = 0; i < _ntexts - TEXTS_E; i++) {
    yr = my_rand(_lay_rec[GRAPH_E].y + 50, _lay_rec[GRAPH_E].h - 50);
    if(i & 1) {
      xr = mid + 60 * i;
    } else {
      xr = mid - 60 * i;
    }
    _gr_rec[i].x = xr;
    _gr_rec[i].y = yr;
    _gr_rec[i].w = _gr_rec[i].h = 60;
  }
}

/** 
 * \brief Initialise tous les paramètres concernant
 * le contexte SDL.
 *
 * \return 0 si tout s'est déroulé correctement, 1 sinon.
 */
static int _init_SDL(void) {
  if(SDL_Init(SDL_INIT_VIDEO) != 0){
    printf("SDL_Init Error: %s\n",  SDL_GetError());
    return 1;
  }

  if(TTF_Init() < 0) {
    printf("TTF_Init Error: %s\n", SDL_GetError());
    return 1;
  }

  _win = SDL_CreateWindow("PCC: Dijkstra", 
                         0, 0, 1280, 760, 
                         SDL_WINDOW_SHOWN | SDL_WINDOW_FULLSCREEN
                        );
  if(_win == NULL) {
    printf("SDL_CreateWindow Error: %s\n", SDL_GetError());
    SDL_Quit();
    return 1;
  }
  _ren = SDL_CreateRenderer(_win, 
                           -1, 
                           SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
                          );
  if(_ren == NULL){
    SDL_DestroyWindow(_win);
    printf("SDL_CreateRenderer Error: %s\n", SDL_GetError());
    SDL_Quit();
    return 1;
  }

  if(_load_texts())
    return 1;
  _init_graph_coord();
  return 0;
}

/** 
 * \brief Dessine un cercle sous SDL.
 *
 * \param p      point sous SDL (centre du cercle)
 * \param radius rayon du cercle
 * \return color couleur du cercle
 */
static void _draw_circle(SDL_Point center, int radius, SDL_Color color) {
  SDL_SetRenderDrawColor(_ren, color.r, color.g, color.b, color.a);
  for (int w = 0; w < radius * 2; w++) {
    for (int h = 0; h < radius * 2; h++) {
      int dx = radius - w;
      int dy = radius - h;
      if ((dx*dx + dy*dy) <= (radius * radius)) {
        SDL_RenderDrawPoint(_ren, center.x + dx, center.y + dy);
      }
    }
  }
}

/** 
 * \brief Dessine les titres de chaque layout. */
static void _draw_title(void) {
  int i;
  for(i = 0; i < LAYOUTS_E; i++) {
    SDL_RenderDrawRect(_ren, &_lay_rec[i]);
    SDL_RenderCopy(_ren, _txtTex[i], NULL, &_txt_rec[i]);
  }
}

/** 
 * \brief Dessine le nom de chaque vertices. */
static void _draw_vertices(void) {
  int i, off = v_rec.x;
  SDL_Rect v_rect = v_rec;
  for(i = 0; i < _ntexts - TEXTS_E; i++) {
    v_rect.x = off;
    if(IN->vertex == i) {
      SDL_SetRenderDrawColor(_ren, 255, 0, 0, 255);
    } else {
      SDL_SetRenderDrawColor(_ren, 0, 0, 0, 255);
    }
    SDL_RenderDrawRect(_ren, &v_rect);
    // TTF_RenderText_Solid
    SDL_RenderCopy(_ren, _txtTex[i + TEXTS_E], NULL, &v_rect);
    off = v_rect.x + 80;
  }
}

/** 
 * \brief Dessine le chemin pour chaque vertex. 
 *
 * \param v vecteur de liste de successeurs (graphe)
 * \param paths liste des chemins pour un vertex donné
 */
static void _draw_paths(fstack_t ** paths) {
  if(!paths)
    return;
  int i;
  char str[256];
  SDL_Surface * s;
  SDL_Texture * tex;
  for(i = 0; i < GRAPH->nbn; i++) {
    sprintf(str, "%s: %d", strndup(GRAPH->data[paths[i]->index].name, 5), paths[i]->dist);
    s = TTF_RenderText_Solid(_font, str, _black);
    if(s == NULL) {
      SDL_DestroyRenderer(_ren);
      SDL_DestroyWindow(_win);
      printf("Error: %s\n", SDL_GetError());
      SDL_Quit();
      return;
    }
    SDL_Rect r = _tpt_rec;
    r.y = (r.y + 50 * i) + 100;
    SDL_Point p = {r.x + r.w/2, r.y + + r.h/2};
    if(IN->paths == i) _draw_circle(p, 5, _red);
    else _draw_circle(p, 5, _black);
    r.x += 100;
    tex = SDL_CreateTextureFromSurface(_ren, s);
    SDL_RenderCopy(_ren, tex, NULL, &r);
    SDL_FreeSurface(s);
  }
}

/** 
 * \brief Dessine les arcs entre chaque vertex (utilisant
 * la matrice compacte).
 *
 * \param paths liste des chemins pour un vertex donné
 */
static void _draw_edge_mat(fstack_t ** paths) {
  int i = 0, nd, nhd;

  char str[20];
  SDL_Surface * s;
  SDL_Texture * tex;
  SDL_Rect r, r0;

  while (i < GRAPH->mat.nba) {
    nd = GRAPH->mat.c[i].n;
    do {
      nhd = GRAPH->mat.c[i].s;
      r.x = _gr_rec[nd].x + _gr_rec[nd].w / 2;
      r.y = _gr_rec[nd].y + _gr_rec[nd].h / 2;
      r.w = _gr_rec[nhd].x + _gr_rec[nhd].w / 2;
      r.h = _gr_rec[nhd].y + _gr_rec[nhd].h / 2;
      SDL_SetRenderDrawColor(_ren, _black.r, _black.g, _black.b, 255);
      SDL_RenderDrawLine(_ren, r.x, r.y, r.w, r.h);

      r0.x = (r.x + r.w) / 2;
      r0.y = (r.y + r.h) / 2;
      r0.w = r.w;
      r0.h = r.h;
      SDL_SetRenderDrawColor(_ren, _red.r, _red.g, _red.b, 150);
      SDL_RenderDrawLine(_ren, r0.x, r0.y, r0.w, r0.h);

      // Value
      sprintf(str, "%d", GRAPH->mat.c[i].val);
      s = TTF_RenderText_Solid(_font, str, _red);
      if(s == NULL) {
        SDL_DestroyRenderer(_ren);
        SDL_DestroyWindow(_win);
        printf("Error: %s\n", SDL_GetError());
        SDL_Quit();
        return;
      }
      r0.w = r0.h = 20;
      tex = SDL_CreateTextureFromSurface(_ren, s);
      SDL_RenderCopy(_ren, tex, NULL, &r0);
      SDL_FreeSurface(s);
      i++;
    } while(i < GRAPH->mat.nba && GRAPH->mat.c[i].n == GRAPH->mat.c[i-1].n);
  }
}

/** 
 * \brief Dessine les arcs entre chaque vertex (utilisant
 * la matrice compacte).
 *
 * \param paths liste des chemins pour un vertex donné
 */
static void _draw_edge_lis(fstack_t ** paths) {
  int i;
  list_t * l;

  char str[20];
  SDL_Surface * s;
  SDL_Texture * tex;
  SDL_Rect r, r0;
  for(i = 0; i < GRAPH->nbn; i++) {
    l = GRAPH->vec.n[i].l;
    while(l) {
      r.x = _gr_rec[i].x + _gr_rec[i].w / 2;
      r.y = _gr_rec[i].y + _gr_rec[i].h / 2;
      r.w = _gr_rec[l->an->in].x + _gr_rec[l->an->in].w / 2;
      r.h = _gr_rec[l->an->in].y + _gr_rec[l->an->in].h / 2;
      SDL_SetRenderDrawColor(_ren, _black.r, _black.g, _black.b, 255);
      SDL_RenderDrawLine(_ren, r.x, r.y, r.w, r.h);

      r0.x = (r.x + r.w) / 2;
      r0.y = (r.y + r.h) / 2;
      r0.w = r.w;
      r0.h = r.h;
      SDL_SetRenderDrawColor(_ren, _red.r, _red.g, _red.b, 150);
      SDL_RenderDrawLine(_ren, r0.x, r0.y, r0.w, r0.h);

      // Value
      sprintf(str, "%d", l->v);
      s = TTF_RenderText_Solid(_font, str, _red);
      if(s == NULL) {
        SDL_DestroyRenderer(_ren);
        SDL_DestroyWindow(_win);
        printf("Error: %s\n", SDL_GetError());
        SDL_Quit();
        return;
      }
      r0.w = r0.h = 20;
      tex = SDL_CreateTextureFromSurface(_ren, s);
      SDL_RenderCopy(_ren, tex, NULL, &r0);
      SDL_FreeSurface(s);
      l = l->succ;
    }
  }
}

/** 
 * \brief Dessine les layouts mis en valeur par le curseur.
 */
static void _draw_hl(void) {
  int i;
  for(i = 0; i < LAYOUTS_E; i++) {
    SDL_Rect r = _lay_rec[i];
    if(_element_target(r.x, r.y, r.x + r.w, r.y + r.h)) {
      SDL_SetRenderDrawColor(_ren,
        _yellow.r, _yellow.g, _yellow.b, _yellow.a);
      SDL_RenderDrawRect(_ren, &r);
    }
  }
}

/** 
 * \brief Dessine les edges (arcs) présents entre chaque
 * vertex.
 *
 * \param paths liste des chemins pour un vertex donné
 */
static void _draw_edge(fstack_t ** paths) {
  if(GRAPH->model == LIS_E) {
    _draw_edge_lis(paths);
  } else {
    _draw_edge_mat(paths);
  }
}

/** 
 * \brief (Re)Initialise le tableau en mettant les 
 * valeurs à 0.
 *
 * \param t tableau
 * \param n taille du tableau
 */
static void _clean_tab(int *t, int n) {
  int i;
  for(i = 0; i < n; i++) {
    t[i] = 0;
  }
}

/** 
 * \brief Vérifie si le curseur de la souris clique dans le 
 * rectangle donné en paramètre.
 *
 * \param x0 coordonnée x0 du rectangle
 * \param y0 coordonnée y0 du rectangle
 * \param y0 longueur x1 du rectangle
 * \param y0 largeur y1 du rectangle
 * \return 1 si le curseur est présent, 0 sinon
 */
static int _element_clicked(int x0, int y0, int x1, int y1) {
  if(IN->mbx > x0 && IN->mby > y0 && IN->mbx < x1 && IN->mby < y1) {
    IN->mbx = IN->mby = 0;
    return 1;
  }
  return 0;
}

/** 
 * \brief Vérifie si le curseur de la souris clible le 
 * rectangle donné en paramètre.
 *
 * \param x0 coordonnée x0 du rectangle
 * \param y0 coordonnée y0 du rectangle
 * \param y0 longueur x1 du rectangle
 * \param y0 largeur y1 du rectangle
 * \return 1 si le curseur est présent, 0 sinon
 */
static int _element_target(int x0, int y0, int x1, int y1) {
  if(IN->mmx > x0 && IN->mmy > y0 && IN->mmx < x1 && IN->mmy < y1) {
    IN->mmx = IN->mmy = 0;
    return 1;
  }
  return 0;
}

/** 
 * \brief Sélectionne un objet représentant un vertex avec
 * la souris.
 */
static void _select_vertices(void) {
  int i, off = v_rec.x;
  SDL_Rect r = v_rec;
  for(i = 0; i < _ntexts - TEXTS_E; i++) {
    r.x = off;
    SDL_RenderDrawRect(_ren, &r);
    SDL_RenderCopy(_ren, _txtTex[i + TEXTS_E], NULL, &r);
    if(_element_clicked(r.x, r.y, r.x + r.w, r.y + r.h)) {
    IN->vertex = i;
    }
    off = r.x + 80;
  }
}

/** 
 * \brief Sélectionne un objet représentant un chemin avec
 * la souris.
 */
static void _select_paths(void) {
  int i;
  for(i = 0; i < _ntexts - TEXTS_E; i++) {
    SDL_Rect r = _tpt_rec;
    r.x = 890;
    r.w = 390;
    r.y = (r.y + 50 * i) + 100;
    if(_element_clicked(r.x, r.y, r.x + r.w, r.y + r.h)) {
      IN->paths = i;
    }
  }
}

/**
 * \brief Sélectionne le vertex de point d'arrivée.
 */
static int _select_end(fstack_t ** paths) {
  _clean_tab(_vvisited, GRAPH->nbn);
  if(IN->paths != -1) {
    int i;
    while(!empty_stack(paths[IN->paths])) {
      i = pop_stack(paths[IN->paths]);
      _vvisited[i] = 1;
    }
    rewind_stack(paths[IN->paths]);
    return 1;
  }
  return 0;
}

/** \brief Met à jour l'affichage du _rendu. */
static void _update(void) {
  SDL_RenderPresent(_ren);
}
/** \brief Remet à zéro le _rendu. */
static void _clear(void) {
  SDL_SetRenderDrawColor(_ren, 255, 255, 255, SDL_ALPHA_OPAQUE);
  SDL_RenderClear(_ren);
}

/** \brief Libère la mémoire. */
static void _clean(void) {
  int i;
  for(i = 0; i < TEXTS_E + GRAPH->nbn; i++) {
    if(_txtTex[i])
      SDL_DestroyTexture(_txtTex[i]);
  }
  if(_win)
    SDL_DestroyWindow(_win);
  if(_ren)
    SDL_DestroyRenderer(_ren);
  SDL_Quit();
}