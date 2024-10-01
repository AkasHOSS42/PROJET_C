#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#include "encode.h"
#include "decode.h"
#include "sort.h"

/* Par defaut, la fusion de deux lignes est leur concatenation.
 * Si l'option -f est appelee, elle marche de la maniere suivante :
 * si l1=u1x1v1 avec x1 le n-eme champ de l1,
 * et l2=u2x2v2 avec x2=x1=x le m-eme champ de l2,
 * la fusion de l1 et l2 selon les champs n:m est la ligne
 * xu1v1u2v2. */

/* Affiche l sur la sortie erreur standard. */
void erreurLigne(line *l, char *pathname, char sep){
	fprintf(stderr, "%s : ", pathname);
	int i;
	for(i=0; i<l->len-1; i++)
		fprintf(stderr, "%s%c ", l->content[i], sep);
	if(l->len!=0)
		fprintf(stderr, "%s\n", l->content[i]);
}

/* Ajoute le index-eme champ de src a la fin de dst. */
void copyField(line *src, line *dst, int index){
	char *tmp=malloc(sizeof(char)*(strlen(src->content[index])+1));
	strcpy(tmp, src->content[index]);
	appendField(dst, tmp);
}

line *fusion(line *l1, line *l2){
	line *ans=createLine();
	int i;
	for(i=0; i<l1->len; i++)
		copyField(l1, ans, i);
	for(i=0; i<l2->len; i++)
		copyField(l2, ans, i);
	return ans;
}

/* Option -f. */
line *fusion2(line *l1, int index1, line *l2, int index2){
	line *ans=createLine();
	copyField(l1, ans, index1);
	int i;
	for(i=0; i<index1; i++)
		copyField(l1, ans, i);
	for(i=index1+1; i<l1->len; i++)
		copyField(l1, ans, i);
	for(i=0; i<index2; i++)
		copyField(l2, ans, i);
	for(i=index2+1; i<l2->len; i++)
		copyField(l2, ans, i);
	return ans;
}

int main(int nArgs, char **args){
	char *out="a.csm";
	char sep=';';
	char *index=NULL;
	int opt=getopt(nArgs, args, "c:hs:o:f");
	int concat=1; // option f?
	while(opt!=-1){
		switch(opt){
		case 'h' :
			/*afficher aide */
			printf("Utilisation de fusion :\n"
			        "\t-h : fait apparaitre cette aide\n"
			        "\t-o file : ecrit dans le fichier sortie \"file\"\n"
			        "\t-s sep : change le separateur en \"sep\"\n"
			        "\t-c n:m : indique les champs selon lesquels fusionner les lignes. "
			        "n est le champ de la premiere, m de la seconde. (option obligatoire)\n"
			        "\t-f fusionne les lignes de maniere non redondante.\n");
			return 0;
		case 's' : // caractere separateur
			sep=*optarg;
			break;
		case 'o' : // fichier de sortie
			out=optarg;
			break;
		case 'c' : // indices sur lesquels fusionner
			index=optarg;
			break;
		case 'f' :
			concat=0;
			break;
		default :
		    fprintf(stderr, "Erreur : option inconnue.\n");
		    return 0;
		}
		opt=getopt(nArgs, args, "c:hs:o:f");
	}
	if(index==NULL){ // pas d'option c
		fprintf(stderr, "Aucune information sur les indices donnee pour la fusion. Echec.\n");
		return 0;
	}
	if(optind>nArgs-2){
		fprintf(stderr, "Deux fichiers sont attendus en argument.\n");
		return 0;
	}
	
	/* Recuperation des indices. */
	int i1=-1;
	int i2=-1;
	int i=0;
	while(index[i]!=':'&&index[i]!='\0')
		i++;
	if(i>0&&index[i]!='\0'&&index[i+1]!='\0'){
		index[i]='\0';
		i1=atoi(index);
		i2=atoi(index+(i+1));
	}
	if(i2<0||i1<0){
		fprintf(stderr, "Les indices indiques sont invalides. Echec de la fusion.\n");
		return 0;
	}
	
	csm *gauche=readCSM(args[optind]);
	if(gauche==NULL){
	    fprintf(stderr, "Erreur de lecture du premier argument.\n");
	    return 0;
	}
	csm *droite=readCSM(args[optind+1]);
	if(droite==NULL){
	    fprintf(stderr, "Erreur de lecture du second argument.\n");
	    freeCSM(gauche);
	    return 0;
	}
	if(sort(gauche, i1, 1)==0 || sort(droite, i2, 1)==0){
	    fprintf(stderr, "Echec de la fusion.\n");
	    freeCSM(gauche);
	    freeCSM(droite);
	    return 0;
	}
	
	csm *output=createCSM();
	int jGauche=0;
	int jDroite=0;
	int diff;
	
	/* Les csm sont tries. A chaque fois on a
	 * les deux minimums de chaque csm.
	 * si ils ne sont pas equivalents,
	 * le plus petit des deux n'est equivalent a aucune ligne
	 * sinon, on les fusionne. */
	while(jGauche!=gauche->len||jDroite!=droite->len){
		if(jGauche==gauche->len){ // plus de lignes dans gauche
			for(; jDroite<droite->len; jDroite++)
				erreurLigne(droite->content[jDroite], args[optind+1], sep);
			break;
		}
		if(jDroite==droite->len){ // plus de lignes dans droite
			for(; jGauche<gauche->len; jGauche++)
				erreurLigne(gauche->content[jGauche], args[optind], sep);
			break;
		}
		diff=strcmp((gauche->content[jGauche])->content[i1], (droite->content[jDroite])->content[i2]);
		if(diff<0) // ligne de gauche plus petite
			erreurLigne(gauche->content[jGauche++], args[optind], sep);
		else if(diff>0) // ligne de droite plus petite
			erreurLigne(droite->content[jDroite++], args[optind+1], sep);
		else if(concat) // equivalence des lignes
			appendLine(output, fusion(gauche->content[jGauche++], droite->content[jDroite++]));
		else // option -f
			appendLine(output, fusion2(gauche->content[jGauche++], i1, droite->content[jDroite++], i2));
	}
	
	if(writeCSM(out, output, sep)==0)
	    fprintf(stderr, "Erreur d'ecriture.\n");
	freeCSM(gauche);
	freeCSM(droite);
	freeCSM(output);
	return 1;
}
