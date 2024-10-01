#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "csm.h"
#include "decode.h"
#include "encode.h"

/* 
 * Fabrique un tableau d'entier correspondant 
 * aux lignes des champs (tab[0] correspond à la taille du tableau)
 * retourne le pointeur vers ce tableau
 */
int *getchamps(char *arg)
{
    int i = 1;
    int *tab = malloc(sizeof(int)*2);
    char *tmp;

    /* tab[0] = longueur du tableau */
    tab[0] = 2;
    /* Initialisation de strtok */
    tmp = strtok(arg, ",");
    tab[i++] = atoi(tmp);
    
    while((tmp = strtok(NULL, ",")) != NULL)
    {
	if(i == tab[0])
	{
	    tab[0] *= 2;
	    tab = realloc(tab, sizeof(int)*tab[0]);
	}
	tab[i++] = atoi(tmp);
    }
    tab[0] = i;

    return tab;
}

int main(int arc, char **argv)
{
    int i, j, len, opt;
    int *tab = NULL;
    csm *csrc, *cdst;
    line *l;
    char *out = "a.csm";
    char *str;
    char sep = ';';

    while((opt = getopt(arc, argv, "ho:s:l:")) != -1)
    {
	switch(opt)
	{
	case 'h':
	    printf("Utilisation de %s :\n"
	           "\t-h : fait apparaitre cette aide\n"
	           "\t-o file : écrit dans le fichier sortie \"file\"\n"
	           "\t-s sep : change le séparateur en \"sep\"\n"
	           "\t-l n1,...,nk : choisit les champs n1 à nk (option obligatoire)\n", argv[0]+2);
	    return 0;

	case 'o':
	    out = optarg;
	    break;

	case 's':
	    sep = optarg[0];
	    break;

	case 'l':
	    tab = getchamps(optarg);
	    break;

	/* Gestion erreur optget */
	default:
	    return 1;
	}
    }
    /* 
     * L'option '-l' est obligatoire,
     * autrement le programme ne marche pas
     */
    if(tab == NULL)
    {
        fprintf(stderr, "erreur : option '-l' absent\n");
	    return 1;
    }

    
    /* Début procédure select */
    /* Gestion cas particulier : "aucun fichier d'entrée": 
     * on met l'entrée standart par défaut
     */
    csrc = (optind == arc) ? entreCSM() : readCSM(argv[optind]);
    cdst = createCSM();

    /* Parcours des lignes de struct csm */
    for(i = 0; i < csrc -> len; i++)
    {
	l = createLine();
	for(j = 1; j < tab[0]; j++)
	{
	    /* Gestion erreur : ligne inexistante */
	    if(tab[j] > (csrc -> content[i]) -> len)
	    {
		fprintf(stderr, "Le champs %d de la ligne %d n'existe pas\n", tab[j], i);
		return 1;
	    }
	    /* Copie des champs choisis dans la ligne */
	    len = strlen((csrc -> content[i]) -> content[tab[j]]) + 1;
	    str = malloc(len * sizeof(char));
	    str = strcpy(str, (csrc -> content[i]) -> content[tab[j]]);
	    str[len-1] = '\0';
	    appendField(l, str);
	}
	/* On ajoute la ligne tab[i] dans le csm */
	appendLine(cdst, l);
    }
    writeCSM(out, cdst, sep);
    freeCSM(csrc);
    freeCSM(cdst);
    
    return 0;
}
