typedef struct graph graph_t;
typedef struct driver driver_t;
#include "list.h"

struct graph {
	int model;
	data_t * data;
};

enum mode_e { LIS_E, MAT_E, NB_MODELS };

void     init_graph(const driver_t*, char * filename);
char *   read_file(char * filename);
void     normalize(char * s);
data_t * tokenize(char * t, int * size);
void     print_data(data_t * data, int n);