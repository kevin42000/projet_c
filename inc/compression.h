#ifndef DEF_COMPRESSION
#define DEF_COMPRESSION

void creer_fichier(char *fichier, char *fichier_original, noeud *alphabet[]);

void decompression(noeud *alphabet[256], char *fichier);

void creation_fichier(noeud *alphabet[256], char *fichier, char *fichier_compresse);

#endif