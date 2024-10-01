/* Fonctions de lecture, qui lisent un fichier
 * .csm et en font un struct csm. */

#ifndef _DECODE_H
#define _DECODE_H

/* Lit le fichier de nom pathname
 * et renvoie le struct csm induit.
 * renvoie NULL si une erreur survient. */
csm *readCSM(char *pathName);

/* Lit un csm a partir de l'entree standard. */
csm *entreCSM();

#endif
