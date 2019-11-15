#ifndef _PARSER_H_
#define _PARSER_H_

typedef struct data data_t;
struct data {
  char * room;
  char ** neigh;
  int * v;
  int nbn;
};

char * read_file(char * filename);
void normalize(char * s);
data_t * tokenize(char * t, int * size);
void print_data(data_t * data, int n);

#endif
