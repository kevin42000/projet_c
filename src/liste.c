
#include <stdio.h>
#include <stdlib.h>
#include "../inc/mon_erreur.h"
#include "../inc/liste.h"

liste liste_vide(){
    return NULL;
}

int est_liste_vide(liste l){
    return (l==liste_vide());
}

liste inserer_element_liste(liste l,element elem){
    liste lnew = (liste)malloc(sizeof(struct_cellule));
    if(lnew == NULL)exit(-2);
    lnew->objet=elem;
    lnew->suivant=l;
    return lnew;
}

element renvoie_premier_element_liste(liste l){
    if(est_liste_vide(l)){
        mon_erreur("[renvoie_premier_element_liste] LISTE VIDE \n");
    }
    return l->objet;
}

liste supprimer_premier_liste(liste l){
    liste lsuivant = l->suivant;
    free(l);
    return lsuivant;
}

void afficher_liste(liste l){
    liste c = l;
    if(est_liste_vide(l)){
        printf("[ ]\n");
        return;
    }
    printf("[ ");
    while(c->suivant){
        printf("%d, ",c->objet);
        c = c->suivant;
    }
    printf("%d ]\n",c->objet);
}