#include <stdio.h>
#include <ctype.h>
#include "csm.h"

/* Fonctions de lecture, qui lisent un fichier
 * .csm et en font un struct csm.
 * On supposera toujours que le fichier se termine
 * par un saut de ligne de fin de fichier. */

/* Extrait un champ d'un fichier csm.
 * On procede caractere par caractere.
 * Renvoie le dernier caractere lu.
 * On agira differemment si ce caractere
 * est le caractere separateur, un saut de ligne
 * ou EOF. Le champ est ajoute a l. */
char readField(FILE *input, char sep, line *l){
  char c=fgetc(input);
  char *ans=malloc(sizeof(char)); // ans est free quand l le sera
  *ans='\0';
  /* On realloc-era ans si la taille du champ depasse sa taille.
   * On ne peut pas savoir la taille du champ a l'avance, donc
   * c'est la seule solution. */
  int size=1;
  int currentSize=0;
  /* On doit supprimer les espaces au debut
   * et a la fin du champ. */
  int espaces_superflus=1;
  while(c!=sep&&c!='\n'&&c!=EOF){
    if(!isspace(c))
      espaces_superflus=0;
    else if(espaces_superflus){
      c=fgetc(input);
      continue;
    }
    if(++currentSize==size){ // on doit realloc
      size*=2;
      ans=realloc(ans, size*sizeof(char));
    }
    ans[currentSize-1]=c;
    c=fgetc(input);
  }
  /* On veut maintenant supprimer les espaces superflus
   * apres le champ. */
  while(currentSize>0&&isspace(ans[currentSize-1]))
    currentSize--;
  ans[currentSize]='\0';
  appendField(l, ans);
  return c;
}

/* Extrait une ligne d'un fichier csm.
 * Retourne Vrai si la fin de fichier est atteinte,
 * Faux sinon. Ajoute la ligne extraite a c. */
int readLine(FILE *input, char sep, csm *c){
  char last=sep;
  line *ans=createLine();
  while(last==sep)
    last=readField(input, sep, ans);
  appendLine(c, ans);
  return last==EOF;
}

/* On doit ignorer les commentaires. */
void ignoreLine(FILE *input){
  char c=fgetc(input);
  while(c!='\n'&&c!=EOF)
    c=fgetc(input);
}

/* Lit un csm a partir d'un fichier.
 * Renvoie un struct csm. */
csm *readFrom(FILE *input){
  char sep=fgetc(input);
  while(sep=='#'){ // ligne de commentaire
    ignoreLine(input);
    sep=fgetc(input);
  }
  if(sep=='\n')
    /* Pas de caractere separateur
       specifie : on prends celui par defaut. */
    sep=';';
  else if(fgetc(input)!='\n'){
    /* On est deja dans une ligne !
     * Il faut revenir en arriere. */
    sep=';';
    fseek(input, -2, SEEK_CUR);
  }

  csm *ans=createCSM();
  int quit=0;
  /* On lit les lignes une par une. */
  while(!quit)
    quit=readLine(input, sep, ans);
  /* On suppose que la derniere ligne est le saut
   * de ligne de fin de fichier. Elle est donc inutile
   * pour notre struct */
  freeLine(ans->content[ans->len-1]);
  ans->len--;
  return ans;
}

/* Lit le fichier de nom pathname
 * et renvoie le struct csm induit.
 * renvoie NULL si une erreur survient. */
csm *readCSM(char *pathName){
  FILE *input=fopen(pathName, "r");
  /* Gestion d'erreur d'ouverture. */
  if(input==NULL){
    perror("");
    return NULL;
  }
  csm *ans=readFrom(input);
  fclose(input);
  return ans;
}

/* Lit un csm a partir de
 * l'entree standard. */
csm *entreCSM(){
	return readFrom(stdin);
}