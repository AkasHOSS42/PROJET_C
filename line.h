#ifndef _LINE_H
#define _LINE_H

#include <stdlib.h>

/* Une sorte de tableau a taille variable de champs. */
typedef struct {
  int len;
  int maxLen;
  char **content; // les champs
} line;

line *createLine();

/* Ajoute un champ en fin de ligne. */
void appendField(line *l, char *field);

void freeLine(line *l);

line *copyLine(line *l);

#endif
