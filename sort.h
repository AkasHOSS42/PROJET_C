/* Trie c selon le champ field.
 * lex -> ordre lexicographique 1; numerique 0
 * Renvoie 0 si une erreur survient, ie des lignes
 * sont trop petites. */
int sort(csm *c, int field, int isLex);