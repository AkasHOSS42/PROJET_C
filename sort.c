#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "csm.h"

/* On va utiliser des pointeurs de fonctions
 * pour faire appel a qsort.
 * On triera toujours selon le premier champ. */

int compareLineLex(const void *l1, const void *l2){
    return strcmp((*(line**)l1)->content[0], (*(line**)l2)->content[0]);
}

/* Les champs vides sont mis a la fin.
 * Les champs non vides alphabatiques sont interpretes
 * comme un 0. Les champs "abs" sont mis entre les champs
 * vides et les autres. */
int compareLineNum(const void *ligne1, const void *ligne2){
    line *l1=*(line**)ligne1;
    line *l2=*(line**)ligne2;
    char *s1=l1->content[0];
    char *s2=l2->content[0];
    if(strlen(s1)==0){ // champ vide : lignes de fin ie element maximal
	if(strlen(s2)==0)
	    return 0;
	else
	    return 1;
    }
    if(strlen(s2)==0)
	return -1;
    if(strcmp(s1, "abs")==0){ // abs : entre les champs vides et les notes
	if(strcmp(s2, "abs")==0)
	    return 0;
	else
	    return 1;
    }
    if(strcmp(s2, "abs")==0)
	return -1;
    return atoi(s1)-atoi(s2);
}

/* Trie c selon le champ field.
 * lex -> ordre lexicographique 1; numerique 0
 * Renvoie 0 si une erreur survient, ie des lignes
 * sont trop petites. */
int sort(csm *c, int field, int isLex){
    /* Premier parcours pour verifier les erreurs. */
    int i;
    for(i=0; i<c->len; i++){
        if((c->content[i])->len<field+1){
            fprintf(stderr, "Erreur : csm mal formate.\n");
            return 0;
        }
    }

    int (*cmp) (const void*, const void*)=&compareLineNum;
    if(isLex)
	cmp=&compareLineLex;
    char *tmp;
	
    /* On echange le champ 0 de chaque ligne
     * avec le champ field. */
    for(i=0; i<c->len; i++){
	tmp=(c->content[i])->content[0];
	(c->content[i])->content[0]=(c->content[i])->content[field];
	(c->content[i])->content[field]=tmp;
    }
	
    /* On trie selon le champ 0. */
    qsort(c->content, c->len, sizeof(line*), cmp);
	
    /* On echange le champ field de chaque ligne
     * avec le champ 0. */
    for(i=0; i<c->len; i++){
	tmp=(c->content[i])->content[field];
	(c->content[i])->content[field]=(c->content[i])->content[0];
	(c->content[i])->content[0]=tmp;
    }
    return 1;
}
