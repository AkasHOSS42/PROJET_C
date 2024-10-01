#ifndef _CSM_H
#define _CSM_H

#include "line.h"

/* Un tableau a taille variable de lignes. */
typedef struct{
  int len;
  int maxLen;
  line **content;
} csm;

csm *createCSM();

void appendLine(csm *c, line *l);

void freeCSM(csm *c);

#endif

