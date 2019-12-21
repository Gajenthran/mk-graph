/*!
 * \file parser.h
 * \brief Header de parser.c
 * \author PANCHALINGAMOORTHY Gajenthran
 * \date 2 Décembre 2020
 */
#ifndef _PARSER_H_
#define _PARSER_H_

typedef struct data data_t;

struct data {
  char * name;   // chambre
  char ** neigh; // chambre voisines
  int * v;       // valeur pour passer d'une chambre à une autre
  int nbn;       // nombre de chambres
};

char *    read_file(char * filename);
void      normalize(char * s);
data_t *  tokenize(char * t, int * size);

#endif