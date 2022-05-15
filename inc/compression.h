#ifndef DEF_COMPRESSION
#define DEF_COMPRESSION
#include "arbre.h"
/**
 * @brief Crée le fichier compressé
 * 
 * @param fichier le chemin du fichier compressé
 * @param fichier_original le chemin du fichier original
 * @param alphabet l'arbre de huffman
 */
void creer_fichier(char *fichier, char *fichier_original, noeud *alphabet[]);

/**
 * @brief lecture de l'en-tête du fichier compressé
 * 
 * @param alphabet l'arbre de huffman à completer
 * @param fichier le chemin du fichier compressé
 */
void decompression(noeud *alphabet[MAX_CHAR], char *fichier);
/**
 * @brief Décompresse fichier_compresses
 * 
 * @param alphabet l'arbre de huffman
 * @param fichier le chemin du fichier à créer
 * @param fichier_compresse le chemin de l'archive
 */
void creation_fichier(noeud *alphabet[MAX_CHAR], char *fichier, char *fichier_compresse);

#endif