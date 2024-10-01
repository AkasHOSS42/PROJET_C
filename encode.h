#ifndef _ENCODE_H
#define _ENCODE_H

/* Fonctions d'ecriture, qui transforment un struct csm en fichier. */

#include "csm.h"

/* Ecrit le struct csm *c dans le fichier de nom
 * pathname. Le fichier est cree si il n'existe pas.
 * sep est le caractere separateur a utiliser. */
int writeCSM(char *pathname, csm *c, char sep);

#endif
