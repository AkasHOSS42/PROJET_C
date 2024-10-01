#include "line.h"

/* Taille max d'un csm a la creation. */
#define DEFAULT_SIZE2 1

/* Un tableau a taille variable de lignes. */
typedef struct{
  int len;
  int maxLen;
  line **content;
} csm;

csm *createCSM(){
  csm *ans=malloc(sizeof(csm));
  ans -> len=0;
  ans -> maxLen=DEFAULT_SIZE2;
  ans -> content=malloc(sizeof(line*)*DEFAULT_SIZE2);
  return ans;
}

void appendLine(csm *c, line *l){
  if(c->len==c->maxLen){ // taille max atteinte
    c->maxLen*=2;
    c->content=realloc(c->content, c->maxLen*sizeof(line*));
  }
  c->content[c->len++]=l;
}

void freeCSM(csm *c){
  int i;
  for(i=0; i<c->len; i++)
    freeLine(c->content[i]);
  free(c);
}
