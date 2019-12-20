#ifndef _GRAPH_H_
#define _GRAPH_H_

typedef struct graph graph_t;
typedef struct driver driver_t;

#include "list.h"
#include "mat.h"
#include "fstack.h"

struct graph {
	vec_t vec;
	mat_t mat;
	data_t * data;
	int model, nbn;
};

enum mode_e { MAT_E, LIS_E, NB_MODELS };

void     init_graph(const driver_t*, char *);
char *   read_file(char * filename);
void     normalize(char * s);
data_t * tokenize(char * t, int * n);
void     print_data(data_t * data, int n);

#endif