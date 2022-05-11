#ifndef DEF_COMPRESSION
#define DEF_COMPRESSION
#include "arbre.h"
void creer_fichier(char *fichier, char *fichier_original, noeud *alphabet[]);

void decompression(noeud *alphabet[MAX_CHAR], char *fichier);

void creation_fichier(noeud *alphabet[MAX_CHAR], char *fichier, char *fichier_compresse);

#endif