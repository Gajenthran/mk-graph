#include <stdio.h>
#include <time.h>
#include <assert.h>
#include "sdl_driver.h"

static const char * texts[] = {
  "Vertices:",
  "Graph:",
  "Paths:"
};

static const SDL_Rect texts_rect[TEXTS_E] = {
  {15, 10, 90, 90},
  {15, 300, 70, 70},
  {930, 300, 70, 70}
};

static const SDL_Rect layouts_rect[LAYOUTS_E] = {
  {0, 0, 1280, 280},
  {0, 280, 890, 680},
  {890, 280, 390, 680}
};

static SDL_Rect vert_rect = {30, 90, 70, 70};

static SDL_Rect * graph_rect = NULL; 
/*! \brief window of the game. */
static SDL_Window *win;
/*! \brief renderer of the game. */
static SDL_Renderer *ren;
/*! \brief all the textures of texts. */
static SDL_Texture ** textTex;
/*! \brief white color. */
static SDL_Color white = { 255, 255, 255, 200 },
                 black = { 0, 0, 0, 200 },
                 red   = { 200, 22, 9, 255},
                 blue  = { 18, 184, 234, 140};
/*! \brief font used for the texts. */
static TTF_Font * font = NULL;
static input_t in;;
static int ntexts = 0;
static int state = 0, old_state = -1;

/*! \brief load all the texts, i.e. transform all the texts into textures. */
static int load_texts(vec_t v) {
  textTex = (SDL_Texture **)malloc((v.nbn + TEXTS_E) * sizeof(*textTex));
  assert(textTex);

  ntexts = v.nbn + TEXTS_E;
  int i;
  font = TTF_OpenFont("files/Arial.ttf", 30);
  SDL_Surface *t;
  for(i = 0; i < TEXTS_E + v.nbn; ++i) {
    if(i < TEXTS_E) {
      t = TTF_RenderText_Solid(font, texts[i], black);
    } else {
      t = TTF_RenderText_Solid(font, v.n[i - TEXTS_E].name, black);
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

/*! \brief initialize all the SDL settings. */
int init_SDL(vec_t v) {
  int i;
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

void draw_circle(SDL_Point center, int radius, SDL_Color color) {
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

void callback(vec_t v) {
  int i;
  fstack_t ** paths = NULL;
  for(;;) {
    if(state != old_state) {
      if(paths) {
        for(i = 0; i < v.nbn; i++)
          free_stack(paths[i]);
      }
      paths = dijkstra(v, state);
      print_pcc(paths, v, state);
      old_state = state;
    }
    clear();
    update_events();
    draw(v, paths);
    update();
  }
}

int my_rand(int min, int max) {
  return (rand()/(double)RAND_MAX) * (max - min) + min;
}

void init_graph_coord(vec_t v) {
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

void draw_title() {
  int i;
  for(i = 0; i < LAYOUTS_E; i++) {
    SDL_RenderDrawRect(ren, &layouts_rect[i]);
    SDL_RenderCopy(ren, textTex[i], NULL, &texts_rect[i]);
  }
}

void draw_vertices() {
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

void draw_paths(vec_t v, fstack_t ** paths) {
  if(!paths)
    return;
  int i;
  char str[256];
  SDL_Surface * s;
  SDL_Texture * tex;
  for(i = 0; i < v.nbn; i++) {
    sprintf(str, "%s: %d", v.n[paths[i]->index].name, paths[i]->dist);
    s = TTF_RenderText_Solid(font, str, black);
    if(s == NULL) {
      SDL_DestroyRenderer(ren);
      SDL_DestroyWindow(win);
      printf("Error: %s\n", SDL_GetError());
      SDL_Quit();
      return;
    }
    SDL_Rect r = {
      layouts_rect[PATHS_E].x + 50,
      (layouts_rect[PATHS_E].y + 50 * i) + 100,
      120,
      50
    };
    tex = SDL_CreateTextureFromSurface(ren, s);
    SDL_RenderCopy(ren, tex, NULL, &r);
    SDL_FreeSurface(s);
  }
}

void draw(vec_t v, fstack_t ** paths) {
  int i, off = vert_rect.x;
  SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
  draw_title();
  draw_vertices();
  for(i = 0; i < ntexts - TEXTS_E; i++) {
    SDL_Point p = {graph_rect[i].x + graph_rect[i].w/2, graph_rect[i].y + + graph_rect[i].h/2};
    draw_circle(p, 20, red);
    SDL_RenderCopy(ren, textTex[i + TEXTS_E], NULL, &graph_rect[i]);
  }
  SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
  draw_edge(v);
  draw_paths(v, paths);
}

void draw_edge(vec_t v) {
  int i, dist[2];
  list_t * l;

  char str[20];
  SDL_Surface * s;
  SDL_Texture * tex;
  for(i = 0; i < v.nbn; i++) {
    l = v.n[i].l;
    while(l) {
      SDL_Rect r = {
        graph_rect[i].x + graph_rect[i].w / 2,
        graph_rect[i].y + graph_rect[i].h / 2,
        graph_rect[l->an->in].x + graph_rect[l->an->in].w / 2,
        graph_rect[l->an->in].y + graph_rect[l->an->in].h / 2
      };
      SDL_SetRenderDrawColor(ren, black.r, black.g, black.b, 255);
      SDL_RenderDrawLine(ren, r.x, r.y, r.w, r.h);

      SDL_SetRenderDrawColor(ren, red.r, red.g, red.b, 255);
      SDL_Rect r0 = {
        (r.x + r.w) / 2,
        (r.y + r.h) / 2,
        r.w,
        r.h,
      };
      SDL_RenderDrawLine(ren, r0.x, r0.y, r0.w, r0.h);
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

void update_events(void) {
  SDL_Event ev;
  srand((unsigned int)time(NULL));
  int t0, t = SDL_GetTicks();
  while(SDL_PollEvent(&ev) != 0) {
    switch(ev.type) {
      case SDL_QUIT:
        quit();
        break;
      case SDL_KEYDOWN:
        if(ev.key.keysym.sym == SDLK_LEFT)
          in.key = -1;
        else if(ev.key.keysym.sym == SDLK_RIGHT)
          in.key = 1;
        break;
      case SDL_MOUSEBUTTONDOWN:
        if(ev.button.button == SDL_BUTTON_LEFT) {
          in.mbx = ev.button.x;
          in.mby = ev.button.y;
        }
      case SDL_MOUSEMOTION:
        in.mmx = ev.button.x;
        in.mmy = ev.button.y;
        break;
      default:
        break;
    }

    t0 = SDL_GetTicks() - t;

    select_vertices();
    if(t0 < 4)
      SDL_Delay(4 - t0);
  }
}

static void select_vertices(void) {
  int i, off = vert_rect.x;
  SDL_Rect v_rect = vert_rect;
  for(i = 0; i < ntexts - TEXTS_E; i++) {
    v_rect.x = off;
    SDL_RenderDrawRect(ren, &v_rect);
    SDL_RenderCopy(ren, textTex[i + TEXTS_E], NULL, &v_rect);
    if(element_clicked(v_rect.x, v_rect.y, v_rect.x + v_rect.w, v_rect.y + v_rect.h)) {
      state = i;
      printf("i:%d\n", i);
    }
    off = v_rect.x + 80;
  }
}

/*! \brief check if the mouse cursor is in the rectangle (coordinates are passed in 
 * parameter) */
int element_targeted(int x0, int y0, int x1, int y1) {
  return (in.mmx > x0 && in.mmy > y0 && in.mmx < x1 && in.mmy < y1);
}

/*! \brief check if the user clicks in the rectangle (coordinates are passed in 
 * parameter) */
int element_clicked(int x0, int y0, int x1, int y1) {
  if(in.mbx > x0 && in.mby > y0 && in.mbx < x1 && in.mby < y1) {
    in.mbx = in.mby = 0;
    return 1;
  }
  return 0;
}

void quit(void) {
  SDL_Quit();
  exit(0);
}

static void update(void) {
  SDL_RenderPresent(ren);
}

void clear(void) {
  SDL_SetRenderDrawColor(ren, 255, 255, 255, SDL_ALPHA_OPAQUE);
  SDL_RenderClear(ren);
}