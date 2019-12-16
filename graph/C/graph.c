#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "parser.h"
#include "graph.h"
#include "driver.h"

graph_t g;

static void callback(void* d) {
}

void init_graph(const driver_t* dr, char * filename) {
	int n;
	char * f = read_file(filename);
	normalize(f);
	data_t * data = tokenize(f, &n);
  	vec_t v = generate_list(data, n);

	dr->init(&g);
	dr->start(callback);
}

