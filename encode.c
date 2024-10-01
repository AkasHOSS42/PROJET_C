#include <stdio.h>
#include "csm.h"

/* Fonctions d'ecriture, qui transforment un struct csm en fichier. */

/* Ecrit dans output la ligne l avec pour caractere separateur sep. */
void writeLine(FILE *output, line *l, char sep){
  int i;
  for(i=0; i<l->len-1; i++){
    fputs(l->content[i], output); // on ecrit le champ
    fputc(sep, output); // on ecrit le caractere separateur
  }
  if(l->len>0)
    /* Le dernier champ n'est pas suivi
     * d'un caractere separateur. */
    fputs(l->content[i], output);
  fputc('\n', output);
}

/* Ecrit le struct csm *c dans le fichier de nom
 * pathname. Le fichier est cree si il n'existe pas.
 * sep est le caractere separateur a utiliser. */
int writeCSM(char *pathname, csm *c, char sep){
  if(c==NULL)
    return 0;
  FILE *output=fopen(pathname, "w");
  if(output==NULL){
    perror("");
    return 0;
  }
  /* Ecriture de la ligne qui donne le caractere separateur. */
  fputc(sep, output);
  fputc('\n', output);
  int i;
  for(i=0; i<c->len; i++) // On ecrit le contenu ligne par ligne.
    writeLine(output, c->content[i], sep);
  fclose(output);
  return 1;
}
