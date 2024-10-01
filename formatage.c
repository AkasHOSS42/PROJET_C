#include <stdio.h>
#include <unistd.h>

#include "csm.h"
#include "encode.h"
#include "decode.h"

int main(int nArgs, char **args){
	int opt=getopt(nArgs, args, "hs:o:");
	char *out="a.csm";
	char sep=';';
	while(opt!=-1){
		switch(opt){
		case 'o' : // fichier de sortie
			out=optarg;
			break;
		case 's' : // caractere seprarateur
			sep=optarg[0];
			break;
		case 'h' :
			/* affichage de l'aide */
			printf("Utilisation de formatage :\n"
			        "\t-h : fait apparaitre cette aide\n"
			        "\t-o file : ecrit dans le fichier sortie \"file\"\n"
			        "\t-s sep : change le separateur en \"sep\"\n");
			return 1;
		default :
		    fprintf(stderr, "Erreur : option inconnue.\n");
		    return 0;
		}
		opt=getopt(nArgs, args, "hs:o:");
	}
	if(optind==nArgs){
		fprintf(stderr, "Un nom de fichier est attendu en argument.\n");
		return 0;
	}
	csm *c=readCSM(args[optind]);
	if(c==NULL){
	    fprintf(stderr, "Erreur de lecture.\n");
	    return 0;
	}
	char *tmp;
	int i;
	int max=0; // le nombre max de champs d'une ligne
	for(i=0; i<c->len; i++)
		if((c->content[i])->len > max)
			max=(c->content[i])->len;
	int j;
	for(i=0; i<c->len; i++)
		for(j=(c->content[i])->len; j<max; j++){ // formatage de la ligne i
			tmp=malloc(sizeof(char));
			*tmp='\0';
			appendField(c->content[i], tmp);
		}
	if(writeCSM(out, c, sep)==0)
	    fprintf(stderr, "Erreur d'ecriture.\n");
	freeCSM(c);
	return 1;
}
