#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "encode.h"
#include "decode.h"
#include "sort.h"

int main(int nArgs, char **args){
    /* On autorise des options inconnues,
     * car l'une des options sera un entier.
     * Si une option invalide est entree
     * (ou qu'aucun champ n'est indique), elle sera
     * interpretee comme 0, et le tri se fera selon le champ 0. */
    int opt=getopt(nArgs, args, ":nhs:o:");
    char *out = "a.csm";
    char sep=';';
    int lex=1; // ordre lexicographique ou non
    while(opt!=-1){
	switch(opt){
	case 's' : // caractere separateur
	    sep=optarg[0];
	    break;
	case 'n' : // ordre numerique
	    lex=0;
	    break;
	case 'h' :
	    /* affichage de l'aide */
	    printf("Utilisation de tri :\n"
		   "\t-h : fait apparaitre cette aide\n"
		   "\t-o file : ecrit dans le fichier sortie \"file\"\n"
		   "\t-s sep : change le separateur en \"sep\"\n"
		   "\t-c : indique le champ selon lequel trier. "
		   "Si l'option n'est pas indiquee, le tri se "
		   "fera selon le champ 0.\n"
		   "\t-n : trie selon l'ordre numerique au lieu de lexicographique.\n");
	    return 1;
	case 'o' : // fichier de sortie
	    out=optarg;
	    break;
	default : // exceptionnellement, on tolere les options inconnues.
	    break;
	}
	opt=getopt(nArgs, args, ":nhs:o:");
    }
    int i;
    int field=0;
    for(i=1; i<optind; i++){
	int tmp=atoi(args[i]);
	if(tmp<0) // champ par lequel faire le tri
	    field=(-1)*tmp;
    }
    csm *c;
    if(optind==nArgs)
	c=entreCSM();
    else
	c=readCSM(args[optind]);
    if(c==NULL){
	fprintf(stderr, "Erreur de lecture.\n");
	return 0;
    }
    if(sort(c, field, lex)==0){
	fprintf(stderr, "Echec du tri.\n");
	freeCSM(c);
	return 0;
    }
    if(writeCSM(out, c, sep)==0)
	fprintf(stderr, "Erreur d'ecriture.\n");
    freeCSM(c);
    return 1;
}
