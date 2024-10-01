#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "csm.h"
#include "decode.h"
#include "encode.h"

/* forme d'un double : "xx.xx", soit 5 caractères maximum 
 * (on supposera que la note sera comprise entre 0.00 et 20.00 inclus)
 */
#define TAILLEMAXDOUBLE 5

/* Gestion erreur : strtod a échoué */
void checkerror(double d, char *error)
{
    if(strcmp(error, "") != 0)
    {
	printf("double = %f\n", d);
	printf("string = %s\n", error);
	printf("erreur : strtod a échoué\n");
	exit(1);
    }
}

/* Convertie le tableau de caractères en un tableau de floats avec :
 * pour i allant de 1 à tab[0]/2 :
 * tab[2i] = ci : numéro de champs
 * tab[2i+1] = ki : coefficient associé
 *
 * Comme pour select, on posera tab[0] comme la taille du tableau
 */
double *getformule(char *arg)
{
    int i = 1;
    double *tab = malloc(sizeof(double)*3);
    char *tmp;
    char *error;
    
    tab[0] = 3;
    /* Initialisation de strtok */
    tmp = strtok(arg, "*");
    tab[i++] = strtod(tmp, &error);
    checkerror(tab[i-1], error);
    
    tmp = strtok(NULL, "+");
    tab[i++] = strtod(tmp, &error);
    checkerror(tab[i-1], error);
    
    while((tmp = strtok(NULL, "*")) != NULL)
    {
	if(i+1 >= tab[0])
	{
	    tab[0] *= 2;
	    tab = realloc(tab, sizeof(double) * tab[0]);
	}
	/* Numéro de champ "ci" */
	tab[i++] = strtod(tmp, &error);
	checkerror(tab[i-1], error);
	
	/* Gestion erreur de forme : "ci *" */
	if((tmp = strtok(NULL, "+")) == NULL)
	{
	    printf("erreur : formule mal définie\n");
	    exit(1);
	}
	/* Coefficient ki */
	tab[i++] = strtod(tmp, &error);
	checkerror(tab[i-1], error);
    }
    tab[0] = i;
    return tab;
}

int main(int arc, char **argv)
{
    int i, j, opt;
    double num, den, tmp;
    double *tab = NULL;
    char *res;
    char *error;
    line *l = NULL;
    csm *csrc, *cdst;
    char *out = "a.csm";
    char sep = ';';

    /* Gestion des options */
    while((opt = getopt(arc, argv, "ho:s:m:")) != -1)
    {
	switch(opt)
	{
	/* Option "-h" */
	case 'h':
	    printf("Utilisation de %s :\n"
	           "\t-h : fait apparaitre cette aide\n"
	           "\t-o file : écrit dans le fichier sortie \"file\"\n"
	           "\t-s sep : change le séparateur en \"sep\"\n"
	           "\t-m formule : formule de forme \"c1 * k1 + ... + cj * kj\" (option obligatoire)\n", argv[0]+2);
	    return 0;

	/* Option "-o" */
	case 'o':
	    out = optarg;
	    break;

	/* Option "-s" */
	case 's':
	    sep = optarg[0];
	    break;

	/* Option "-m" (obligatoire) */
	case 'm':
	    tab = getformule(optarg);
	    break;

	/* Gestion erreur : option inexistante ou option sans son argument associé */
	default :
	    fprintf(stderr, "erreur getopt\n");
	    return 1;
	}
    }
    /* Gestion erreur : option "-m" non définie */
    if(tab == NULL)
    {
	fprintf(stderr, "erreur : option '-m' absent\n");
	return 1;
    }


    /* Début de la procédure moyenne_ponderee */
    /* Gestion cas particulier : "aucun fichier d'entrée"
     * on met l'entrée standart par défaut
     */
    csrc = (optind == arc) ? entreCSM() : readCSM(argv[optind]);
    cdst = createCSM();

    /* Parcours des lignes du struct csm */
    for(i = 0; i < csrc -> len; i++)
    {
	l = copyLine(csrc -> content[i]);
	den = 0;
	num = 0;
	for(j = 1; j < tab[0]; j+=2)
	{
	    /* Gestion erreur : champs inexistant */
	    if((int)tab[j] > (csrc -> content[i]) -> len)
	    {
		fprintf(stderr, "Le champs %d de la ligne %d n'existe pas\n", (int)tab[j], i);
		return 1;
	    }
	    /* Gestion cas particulier : "abs" */
	    else if(strcmp((csrc -> content[i]) -> content[(int)tab[j]], "abs") == 0)
		continue;
	    
	    /* Gestion erreur : le champs n'est pas un entier */
	    tmp = strtod((csrc -> content[i]) -> content[(int)tab[j]], &error);
	    checkerror(tmp, error);
	    num += tmp * tab[j+1];
	    den += tab[j+1];
	}
	/* Si l'élève a toutes ses notes en abs, 
	 * alors le dénominateur sera égale à 0, 
	 * et donc la valeur de sortie est "abs"
	 */
	res = malloc(sizeof(char) * TAILLEMAXDOUBLE);
	if(den == 0)
	    strcpy(res, "abs");
	else
	    sprintf(res, "%.2f", num / den);

	/*   Ajout du nouveau champs dans la ligne
	 * + ajout de la ligne dans le nouveau CSM
	 */
	appendField(l, res);
	appendLine(cdst, l);
    }
    writeCSM(out, cdst, sep);
    freeCSM(csrc);
    freeCSM(cdst);
    free(tab);
    
    return 0;
}
