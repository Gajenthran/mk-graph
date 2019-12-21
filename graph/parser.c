/*!
 * \file parser.c
 * \brief Parsing des fichiers d'entrée
 * \author PANCHALINGAMOORTHY Gajenthran
 * \date 2 Décembre 2020
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "parser.h"


/** \brief Lire le fichiers de données et retourner 
 * son contenu sous forme de chaîne de caractère.
 *
 * \param filename nom du fichier
 * 
 * \return le contenu des données sous forme de chaîne
 */
char * read_file(char * filename) {
  char * d = NULL;
  size_t size = 0;

  FILE * fp = fopen(filename, "r");
  if(!fp) {
    fprintf(stderr, "Can't open file %s\n", filename);
    exit(0);
  }

  fseek(fp, 0, SEEK_END);
  size = ftell(fp);
  rewind(fp);

  d = (char *)malloc((size + 1) * sizeof *d);
  fread(d, size, 1, fp);

  d[size] = '\0';
  fclose(fp);
  return d;
}

/** \brief Normalise la chaîne en retirant
 * les majuscules et les espaces.
 *
 * \param s chaîne de caractères
 * 
 */
void normalize(char * s) {
  int c = 0;
  while(s[c] != '\0') {
   if(s[c] >= 'A' && s[c] <= 'Z')
    s[c] = s[c] + 32;
   c++;
  }

  const char * d = s;
  while(*s) {
    while(*d == ' ')
      ++d;
    *s++ = *d++;
  }
}

/** \brief Tokenize le contenu du fichiers en le coupant
 * en lignes et en virgule pour récupérer les données 
 * utiles (vertex + successeurs)
 *
 * \param t fichier de données sous forme de chaîne de caractère
 * \param size taille du fichier de données
 * 
 * \return la structure de forme data_t qui représente les données 
 * formalisées
 */
data_t * tokenize(char * t, int * size) {
  int i = 0, j = 0, c = 1;
  while(t[i]) {
    if(t[i] == '\n') c++;
    i++;
  }

  char * tok = strtok(t, "\n");
  char ** lines = (char **)malloc(c * sizeof *lines);
  i = 0;

  while(tok != NULL) {
    lines[i++] = strdup(tok);
    tok = strtok(NULL, "\n");
  }

  data_t * data = (data_t *)malloc(c * sizeof *data);
  assert(data);
  for(i = 0; i < c; i++) {
    data[i].neigh = (char **)malloc(c * sizeof(*data[i].neigh));
    assert(data[i].neigh);
    data[i].v = (int *)malloc(c * sizeof(*data[i].neigh));
    assert(data[i].v);
    tok = strtok(lines[i], ",");
    j = 0;
    while(tok != NULL) {
      if(!j) {
        data[i].name = strdup(tok);
      } else {
        data[i].neigh[j-1] = strdup(tok);
      }
      tok = strtok(NULL, ",");
      j++;
    }
    data[i].nbn = j-1;
  }

  int v = 0;
  for(i = 0; i < c; i++) {
    for(j = 0; j < data[i].nbn; j++) {
      tok = strtok(data[i].neigh[j], "__");
      while(tok != NULL) {
        if(!v) {
          data[i].neigh[j] = strdup(tok);
        } else {
          data[i].v[j] = atoi(tok);
        }
        tok = strtok(NULL, "__");
        v = (v + 1) % 2;
      }
    }
  }
  *size = c;
  return data;
}