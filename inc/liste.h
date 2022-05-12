#ifndef _LISTE_H_
#define _LISTE_H_
#include <stdio.h>
#include <stdlib.h>
#include "./mon_erreur.h"
typedef int element;
typedef struct cellule{
    element objet;
    struct cellule * suivant;
}struct_cellule;
typedef struct_cellule * liste;

liste liste_vide();

int est_liste_vide(liste l);

liste inserer_element_liste(liste l,element elem);

element renvoie_premier_element_liste(liste l);

liste supprimer_premier_liste(liste l);

void afficher_liste(liste l);

#endif