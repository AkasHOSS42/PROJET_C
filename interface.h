typedef struct{ // dans line.h
    int len;
    int maxLen;
    char **content;
} line;

typedef struct{ //dans csm.h
    int len;
    int maxLen;
    line **content;
} csm;

void appendField(line *l, char *field); //cf line.h
void appendLine(csm *c, line *l); // cf csm.h

/* lex -> ordre lexicographique 1; numérique 0 */
void sort(csm *c, int field, int isLex); // pas encore code

csm *readCSM(char *pathname); // cf decode.h
/* cf encode.h */
int writeCSM(char *pathname, csm *c, char sep); // c n'est pas free après écriture

void freeCSM(csm *a);