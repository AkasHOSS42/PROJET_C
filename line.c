#include <stdlib.h>
#include <string.h>

/* La taille max a la creation. */
#define DEFAULT_SIZE 1

/* Une sorte de tableau a taille variable de champs. */
typedef struct {
  int len;
  int maxLen;
  char **content;
} line;

line *createLine(){
  line *ans=malloc(sizeof(line));
  ans -> len=0;
  ans -> maxLen=DEFAULT_SIZE;
  ans -> content=malloc(sizeof(char*)*DEFAULT_SIZE);
  return ans;
}

/* Ajoute un champs en fin de ligne. */
void appendField(line *l, char *field){
  if(l->len==l->maxLen){ //taille max atteinte
    l->maxLen*=2;
    l->content=realloc(l->content, l->maxLen*sizeof(char*));
  }
  l->content[l->len++]=field;
}

void freeLine(line *l){
  int i;
  for(i=0; i<l->len; i++)
    free(l->content[i]);
  free(l->content);
  free(l);
}

line *copyLine(line *l){
  line *ans=createLine();
    char *tmp;
    int i;

    /* Copie de content */
    for(i = 0; i < l->len; i++){
	tmp = malloc((strlen(l -> content[i])+1) * sizeof(char));
	strcpy(tmp, l->content[i]);
	appendField(ans, tmp);
    }
    return ans;
}
