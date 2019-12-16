typedef struct driver driver_t;

struct driver {
    const graph_t* graph;
    int  (*init)(const graph_t* graph);
    void (*start)(void(*callback)(void*));
    void (*quit_game)(void);
};

