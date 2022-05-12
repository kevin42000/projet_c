#include "../inc/pile.h"

pile pile_vide(){
    return liste_vide();
}

int est_pile_vide(pile p){
    return est_liste_vide(p);
}

pile empiler(pile p,element e){
    return inserer_element_liste(p,e);
}

element sommet(pile p){
    return renvoie_premier_element_liste(p);
}

pile depiler(pile p){
    return supprimer_premier_liste(p);
}

int taille_pile(pile p){
    int x=0;
    liste cur = p;
    while (cur)
    {
        x++;
        cur = cur->suivant;
    }
    return x;
}