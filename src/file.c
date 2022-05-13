#include "../inc/file.h"

file file_vide(){
    return liste_vide();
}

int est_file_vide(file f){
    return est_liste_vide(f);
}

file enfiler(file f,element e){
    liste lnew = inserer_element_liste(liste_vide(),e);
    liste lcur =f;
    if(est_file_vide(f))return lnew;
    while (lcur->suivant!=liste_vide())
    {
        lcur = lcur->suivant;
    }
    lcur->suivant = lnew;
    return f;
}

element debut_file(file f){
    return renvoie_premier_element_liste(f);
}

file defiler(file f){
    return supprimer_premier_liste(f);
}

int taille_file(file f){
    int x=0;
    liste cur = f;
    while (cur)
    {
        x++;
        cur = cur->suivant;
    }
    return x;
}