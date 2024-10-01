#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "csm.h"
#include "decode.h"
#include "encode.h"

int main(int arc, char **argv)
{
    int i, j, opt;
    int lentmp;
    csm *csrc, *cdst;
    char *out = "a.csm";
    char sep = ';';

    /* Ajouter les options supplémentaires dans le 3ème argument */
    while((opt = getopt(arc, argv, "ho:s:")) != -1)
    {
	/* Pour avoir le dernier indice du dernier argument option */
	switch(opt)
	{
	/* Option "-h" aide */
	case 'h':
	    printf("Utilisation de %s :\n"
	           "\t-h : fait apparaitre cette aide\n"
	           "\t-o file : écrit dans le fichier sortie \"file\"\n"
	           "\t-s sep : change le séparateur en \"sep\"\n", argv[0]+2);
	    return 0;

	/* Option "-o" fichier de sortie */
	case 'o':
	    out = optarg;
	    break;

	/* Option "-s" séparateur */
	case 's':
	    sep = *optarg;
	    break;

	/* Gestion erreur
	 * option qui n'existe pas
	 * ou option sans son argument 
	 */
	default :
	    return 1;
	}
    }
    /* Gestion erreur : aucun fichier d'entrée */
    if(optind == arc)
    {
	    fprintf(stderr, "Aucun fichier d'entrée\n");
	    return 1;
    }
    
    
    /* debut de la procédure union */
    cdst = createCSM();
    /* initialisation de lentmp */
    lentmp = -1;

    /* Parcours des fichiers d'entrée */
    for(i = optind; i < arc; i++)
    {
	/* On transforme le fichier d'entrée en struct csm */
	csrc = readCSM(argv[i]);
	
	/* Si le fichier n'est pas "vide" et que le nombre de champs dans une 
	 * ligne est égal au nombre de lignes des autres fichiers */
	if(csrc -> len != 0 && ((csrc  -> content[0]) -> len == lentmp || lentmp == -1))
	{
	    lentmp = (csrc -> content[0]) -> len;
	}

	/* Gestion erreur : nombre de champs différents */
	else
	{
	    fprintf(stderr, "union échoue, nombre de champs différents\n");
	    return 1;
	}

	/* Parcours des lignes de la struct csm */
	for(j = 0; j < csrc -> len; j++)
	{
	    /* On ajoute la ligne dans le csm */
	    appendLine(cdst, copyLine(csrc -> content[j]));
	}
	free(csrc);
    }
    writeCSM(out, cdst, sep);
    freeCSM(cdst);
    
    return 0;
}
