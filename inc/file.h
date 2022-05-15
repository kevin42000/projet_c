#ifndef _FILE_H_
#define _FILE_H_
#include "liste.h"

typedef liste file;

/**
 * @brief revoie la file vide
 * 
 * @return file 
 */
file file_vide();

/**
 * @brief revoie vrai si la file est file
 * 
 * @param f la file
 * @return int 0 = faux 1 = vrai
 */
int est_file_vide(file f);

/**
 * @brief ajoute l'élément e à la file f
 * 
 * @param f la file
 * @param e l'élément
 * @return file la file
 */
file enfiler(file f,element e);

/**
 * @brief revoie le premier élément de la file
 * 
 * @param f la file
 * @return element l'element
 */
element debut_file(file f);

/**
 * @brief enlève le premier élément de la file
 * 
 * @param f la file
 * @return file la file sn son premier élément
 */
file defiler(file f);

/**
 * @brief Revoie la taille de la file f
 * 
 * @param f la file
 * @return int la taille de la file
 */
int taille_file(file f);

#endif