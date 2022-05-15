#ifndef DEF_OCCURENCE
#define DEF_OCCURENCE
#include "arbre.h"
/**
 * @brief affiche une partie du fichier texte passé en paramètre
 * 
 * @param fichier la chaine de caractère
 */
void affiche_une_partie(char fichier[]);
/**
 * @brief affiche la totalité du fichier texte passé en paramètre
 * 
 * @param fichier la chaine de caractère
 */
void affiche_la_totalite(char fichier[]);
/**
 * @brief affiche une partie du fichier texte passé en paramètre
 * 
 * @param fichier la chaine de caractère
 */
void occurence(FILE *fic, int tab[MAX_CHAR]);

#endif
