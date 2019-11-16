#include <stdio.h>
#include <time.h>
#include <assert.h>
#include "sdl_driver.h"

static int load_texts(vec_t v);
static void init_graph_coord(vec_t v);
static void draw(vec_t v, fstack_t ** paths);
static void draw_circle(SDL_Point center, int radius, SDL_Color color);
static void draw_title(void);
static void draw_vertices(void);
static void draw_paths(vec_t v, fstack_t ** paths);
static void draw_edge(vec_t v, fstack_t ** paths);
static void clean_tab(int *t, int n);
static void update_events(void);
static int element_clicked(int x0, int y0, int x1, int y1);
static void select_vertices(void);
static void select_paths(void);
static void quit(void);
static void update(void);
static void clear(void);

/**
 * \brief Textes représentant les titres présent
 * dans la fenêtre SDL.
 */
static const char * texts[] = {
  "Vertices:",
  "Graph:",
  "Paths:"
};

/**
 * \brief Position des textes représentant les titres 
 * présent dans la fenêtre SDL.
 */
static const SDL_Rect texts_rect[TEXTS_E] = {
  {15, 10, 90, 90},
  {15, 300, 70, 70},
  {930, 300, 70, 70}
};

/**
 * \brief Layouts représentant les différentes parties
 * du programme dans la fenêtre SDL.
 */
static const SDL_Rect layouts_rect[LAYOUTS_E] = {
  {0, 0, 1280, 280},
  {0, 280, 890, 680},
  {890, 280, 390, 680},
};

/**
 * \brief Position de l'affichage du chemin d'un vertex
 * vers un autre.
 */
static SDL_Rect tpaths_rect = {
  830 + 50,
  280,
  120,
  50
};

/** \brief Position de l'affichage des nom de vertices */
static SDL_Rect vert_rect = {30, 90, 70, 70};
/** \brief Les vertices visités */
static int * visited_cir = NULL;
/** \brief Position de l'affichage des vertices */
static SDL_Rect * graph_rect = NULL; 
/** \brief Fenêtre du programme SDL */
static SDL_Window *win;
/** \brief Rendu du programme SDL */
static SDL_Renderer *ren;
/** \brief Liste des textures de texte du programme SDL */
static SDL_Texture ** textTex;
/** \brief Liste des SDL couleurs sous la forme RGB */
static SDL_Color black = { 0, 0, 0, 200 },
                 red   = { 200, 22, 9, 255},
                 blue  = { 18, 184, 234, 140};
/** \brief Police utilisée pour les textes */
static TTF_Font * font = NULL;
/** \brief Représentation des entrées utilisateur */
static input_t in;
/** \brief Nombre de textes présenté dans le programme */
static int ntexts = 0;
/** \brief L'indice du vertex de départ */
static int state = 0, old_state = -1;
/** \brief L'indice du vertex final */
static int pstate = -1, old_pstate = -1;


/** 
 * \brief Initialise tous les paramètres concernant
 * le contexte SDL.
 *
 * \param v vecteur de liste de successeurs (graphe)
 * \return 0 si tout s'est déroulé correctement, 1 sinon.
 */
int init_SDL(vec_t v) {
  if(SDL_Init(SDL_INIT_VIDEO) != 0){
    printf("SDL_Init Error: %s\n",  SDL_GetError());
    return 1;
  }

  if(TTF_Init() < 0) {
    printf("TTF_Init Error: %s\n", SDL_GetError());
    return 1;
  }

  win = SDL_CreateWindow("PCC: Dijkstra", 0, 0, 1280, 760, SDL_WINDOW_SHOWN);
  if(win == NULL) {
    printf("SDL_CreateWindow Error: %s\n", SDL_GetError());
    SDL_Quit();
    return 1;
  }
  ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  if(ren == NULL){
    SDL_DestroyWindow(win);
    printf("SDL_CreateRenderer Error: %s\n", SDL_GetError());
    SDL_Quit();
    return 1;
  }

  if(load_texts(v))
    return 1;
  init_graph_coord(v);
  return 0;
}

/**
 * \brief Fonction de callback qui permet d'utiliser
 * l'algorithme de Dijkstra pour chercher le plus
 * court chemin.
 *
 * \param v vecteur de liste de successeurs (graphe)
 */
void callback(vec_t v) {
  int i;
  fstack_t ** paths = NULL;
  visited_cir = (int *)calloc(0, v.nbn * sizeof(*visited_cir));
  assert(visited_cir);
  while(!in.quit) {
    clear();
    if(state != old_state) {
      if(paths) {
        for(i = 0; i < v.nbn; i++)
          free_stack(paths[i]);
      }
      paths = dijkstra(v, state);
      old_state = state;
    }
    if(pstate >= 0 && pstate != old_pstate) {
      if(state != pstate) {
        clean_tab(visited_cir, v.nbn);
        while(!empty_stack(paths[pstate]))
          visited_cir[pop_stack(paths[pstate])] = 1;
        rewind_stack(paths[pstate]);
      }
      old_pstate = pstate;
    }
    update_events();
    draw(v, paths);
    update();
  }
  quit();
}

/** 
 * \brief Charger tous les textes, càd transformer tous 
 * les textes en textures. 
 *
 * \param v vecteur de liste de successeurs (graphe)
 * \return 0 si tout s'est déroulé correctement, 1 sinon.
 */
static int load_texts(vec_t v) {
  textTex = (SDL_Texture **)malloc((v.nbn + TEXTS_E) * sizeof(*textTex));
  assert(textTex);

  ntexts = v.nbn + TEXTS_E;
  int i;
  font = TTF_OpenFont("files/Roboto-Condensed.ttf", 30);
  SDL_Surface *t;
  for(i = 0; i < TEXTS_E + v.nbn; ++i) {
    if(i < TEXTS_E) {
      t = TTF_RenderText_Solid(font, texts[i], black);
    } else {
      t = TTF_RenderText_Solid(font, strndup(v.n[i - TEXTS_E].name, 5), black);
    }
    if(t == NULL) {
      SDL_DestroyRenderer(ren);
      SDL_DestroyWindow(win);
      printf("Error: %s\n", SDL_GetError());
      SDL_Quit();
      return 1;
    }

    textTex[i] = SDL_CreateTextureFromSurface(ren, t);
    SDL_FreeSurface(t);
  }
  return 0;
}

/** 
 * \brief Initialise les coordonnées pour
 * l'affichage du graphe.
 *
 * \param v vecteur de liste de successeurs (graphe)
 */
static void init_graph_coord(vec_t v) {
  srand(time(NULL));
  int xr, yr, i;
  graph_rect = (SDL_Rect *)malloc(v.nbn * sizeof(*graph_rect));
  assert(graph_rect);
  int mid = (layouts_rect[GRAPH_E].w - 50) / 2;
  for(i = 0; i < ntexts - TEXTS_E; i++) {
    yr = my_rand(layouts_rect[GRAPH_E].y + 50, layouts_rect[GRAPH_E].h - 50);
    if(i & 1) {
      xr = mid + 60 * i;
    } else {
      xr = mid - 60 * i;
    }
    graph_rect[i].x = xr;
    graph_rect[i].y = yr;
    graph_rect[i].w = graph_rect[i].h = 60;
  }
}

/** 
 * \brief Dessine les différentes parties du programmes à
 * savoir les différentes vertices présents dans le graphe,
 * le graphe et les chemins.
 *
 * \param v vecteur de liste de successeurs (graphe)
 * \param paths liste des chemins pour un vertex donné
 */
static void draw(vec_t v, fstack_t ** paths) {
  int i;
  SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
  draw_title();
  draw_vertices();
  for(i = 0; i < ntexts - TEXTS_E; i++) {
    SDL_Point p = {graph_rect[i].x + graph_rect[i].w/2, graph_rect[i].y + + graph_rect[i].h/2};
    if(visited_cir[i])
    draw_circle(p, 20, blue);
    else
    draw_circle(p, 20, red);
    SDL_RenderCopy(ren, textTex[i + TEXTS_E], NULL, &graph_rect[i]);
  }
  SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
  draw_edge(v, paths);
  draw_paths(v, paths);
}

/** 
 * \brief Dessine un cercle sous SDL.
 *
 * \param p      point sous SDL (centre du cercle)
 * \param radius rayon du cercle
 * \return color couleur du cercle
 */
static void draw_circle(SDL_Point center, int radius, SDL_Color color) {
  SDL_SetRenderDrawColor(ren, color.r, color.g, color.b, color.a);
  for (int w = 0; w < radius * 2; w++) {
    for (int h = 0; h < radius * 2; h++) {
      int dx = radius - w;
      int dy = radius - h;
      if ((dx*dx + dy*dy) <= (radius * radius)) {
        SDL_RenderDrawPoint(ren, center.x + dx, center.y + dy);
      }
    }
  }
}

/** 
 * \brief Dessine les titres de chaque layout. */
static void draw_title(void) {
  int i;
  for(i = 0; i < LAYOUTS_E; i++) {
    SDL_RenderDrawRect(ren, &layouts_rect[i]);
    SDL_RenderCopy(ren, textTex[i], NULL, &texts_rect[i]);
  }
}

/** 
 * \brief Dessine le nom de chaque vertices. */
static void draw_vertices(void) {
  int i, off = vert_rect.x;
  SDL_Rect v_rect = vert_rect;
  for(i = 0; i < ntexts - TEXTS_E; i++) {
    v_rect.x = off;
    if(state == i) {
      SDL_SetRenderDrawColor(ren, 255, 0, 0, 255);
    } else {
      SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
    }
    SDL_RenderDrawRect(ren, &v_rect);
    SDL_RenderCopy(ren, textTex[i + TEXTS_E], NULL, &v_rect);
    off = v_rect.x + 80;
  }
}

/** 
 * \brief Dessine le chemin pour chaque vertex. 
 *
 * \param v vecteur de liste de successeurs (graphe)
 * \param paths liste des chemins pour un vertex donné
 */
static void draw_paths(vec_t v, fstack_t ** paths) {
  if(!paths)
    return;
  int i;
  char str[256];
  SDL_Surface * s;
  SDL_Texture * tex;
  for(i = 0; i < v.nbn; i++) {
    sprintf(str, "%s: %d", strndup(v.n[paths[i]->index].name, 5), paths[i]->dist);
    s = TTF_RenderText_Solid(font, str, black);
    if(s == NULL) {
      SDL_DestroyRenderer(ren);
      SDL_DestroyWindow(win);
      printf("Error: %s\n", SDL_GetError());
      SDL_Quit();
      return;
    }
    SDL_Rect r = tpaths_rect;
    r.y = (r.y + 50 * i) + 100;
    SDL_Point p = {r.x + r.w/2, r.y + + r.h/2};
    if(i == pstate) draw_circle(p, 5, red);
    else draw_circle(p, 5, black);
    r.x += 100;
    tex = SDL_CreateTextureFromSurface(ren, s);
    SDL_RenderCopy(ren, tex, NULL, &r);
    SDL_FreeSurface(s);
  }
}

/** 
 * \brief Dessine les edges (arcs) présents entre chaque
 * vertex (arc).
 *
 * \param v vecteur de liste de successeurs (graphe)
 * \param paths liste des chemins pour un vertex donné
 */
static void draw_edge(vec_t v, fstack_t ** paths) {
  int i;
  list_t * l;

  char str[20];
  SDL_Surface * s;
  SDL_Texture * tex;
  SDL_Rect r, r0;
  for(i = 0; i < v.nbn; i++) {
    l = v.n[i].l;
    while(l) {
      r.x = graph_rect[i].x + graph_rect[i].w / 2;
      r.y = graph_rect[i].y + graph_rect[i].h / 2;
      r.w = graph_rect[l->an->in].x + graph_rect[l->an->in].w / 2;
      r.h = graph_rect[l->an->in].y + graph_rect[l->an->in].h / 2;
      SDL_SetRenderDrawColor(ren, black.r, black.g, black.b, 255);
      SDL_RenderDrawLine(ren, r.x, r.y, r.w, r.h);

      r0.x = (r.x + r.w) / 2;
      r0.y = (r.y + r.h) / 2;
      r0.w = r.w;
      r0.h = r.h;
      SDL_SetRenderDrawColor(ren, red.r, red.g, red.b, 150);
      SDL_RenderDrawLine(ren, r0.x, r0.y, r0.w, r0.h);

      // Value
      sprintf(str, "%d", l->v);
      s = TTF_RenderText_Solid(font, str, red);
      if(s == NULL) {
        SDL_DestroyRenderer(ren);
        SDL_DestroyWindow(win);
        printf("Error: %s\n", SDL_GetError());
        SDL_Quit();
        return;
      }
      r0.w = r0.h = 20;
      tex = SDL_CreateTextureFromSurface(ren, s);
      SDL_RenderCopy(ren, tex, NULL, &r0);
      SDL_FreeSurface(s);
      l = l->succ;
    }
  }
}

/** 
 * \brief (Re)Initialise le tableau en mettant les 
 * valeurs à 0.
 *
 * \param t tableau
 * \param n taille du tableau
 */
static void clean_tab(int *t, int n) {
  int i;
  for(i = 0; i < n; i++)
    t[i] = 0;
}

/** 
 * \brief Met à jour les entrées utilisateurs.
 */
static void update_events(void) {
  SDL_Event ev;
  srand((unsigned int)time(NULL));
  int t0, t = SDL_GetTicks();
  while(SDL_PollEvent(&ev) != 0) {
    switch(ev.type) {
      case SDL_QUIT:
        in.quit = 1;
        break;
      case SDL_MOUSEBUTTONDOWN:
        if(ev.button.button == SDL_BUTTON_LEFT) {
          in.mbx = ev.button.x;
          in.mby = ev.button.y;
        }
        break;
      default:
        break;
    }

    t0 = SDL_GetTicks() - t;

    select_vertices();
    select_paths();
    if(t0 < 4)
      SDL_Delay(4 - t0);
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
static int element_clicked(int x0, int y0, int x1, int y1) {
  if(in.mbx > x0 && in.mby > y0 && in.mbx < x1 && in.mby < y1) {
    in.mbx = in.mby = 0;
    return 1;
  }
  return 0;
}

/** 
 * \brief Sélectionne un objet représentant un vertex avec
 * la souris.
 */
static void select_vertices(void) {
  int i, off = vert_rect.x;
  SDL_Rect v_rect = vert_rect;
  for(i = 0; i < ntexts - TEXTS_E; i++) {
    v_rect.x = off;
    SDL_RenderDrawRect(ren, &v_rect);
    SDL_RenderCopy(ren, textTex[i + TEXTS_E], NULL, &v_rect);
    if(element_clicked(v_rect.x, v_rect.y, v_rect.x + v_rect.w, v_rect.y + v_rect.h)) {
      state = i;
    }
    off = v_rect.x + 80;
  }
}

/** 
 * \brief Sélectionne un objet représentant un chemin avec
 * la souris.
 */
static void select_paths() {
  int i;
  for(i = 0; i < ntexts - TEXTS_E; i++) {
    SDL_Rect r = tpaths_rect;
    r.x = 890;
    r.w = 390;
    r.y = (r.y + 50 * i) + 100;
    if(element_clicked(r.x, r.y, r.x + r.w, r.y + r.h)) {
      pstate = i;
    }
  }
}

/** \brief Quitte le programme. */
static void quit(void) {
  SDL_Quit();
  exit(0);
}
/** \brief Met à jour l'affichage du rendu. */
static void update(void) {
  SDL_RenderPresent(ren);
}
/** \brief Remet à zéro le rendu. */
static void clear(void) {
  SDL_SetRenderDrawColor(ren, 255, 255, 255, SDL_ALPHA_OPAQUE);
  SDL_RenderClear(ren);
}