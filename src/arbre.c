#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "../inc/arbre.h"

/*crée une structure noeud pour un caractère contenu dans le fichier*/
noeud * creer_feuille(int *tab, int index){
    noeud *arbre_huffman;
    if(tab[index] == 0){
        return NULL;
    }
    arbre_huffman = (noeud*)malloc(sizeof(noeud));
    if(arbre_huffman == NULL){
        printf("Erreur allocation mémoire.\n");
        return NULL;
    }
    arbre_huffman->caractere = index;
    arbre_huffman->occurence = tab[index];
    arbre_huffman->codage[0] = '\0';
    arbre_huffman->nb_bits = 0;
    arbre_huffman->gauche = NULL;
    arbre_huffman->droit = NULL;
    return arbre_huffman;
}

/*récupère les 2 plus faibles occurences*/
void parcours_tableau(noeud *arbre_huffman[256], int taille, int *x, int *y){
    int i = 0, j = 0;
    int x1 = INT_MAX, x2 = INT_MAX;
    *x = 256;
    *y = 256;
    for(i=0;i<taille;i++){
        if(arbre_huffman[i] != NULL){
            if(arbre_huffman[i]->occurence < x1){
                j = *x;
                x1 = arbre_huffman[i]->occurence;
                *x = i;
            }
            if(j > 0 && j < 256 && arbre_huffman[j]->occurence < x2){
                x2 = arbre_huffman[j]->occurence;
                *y = j;
            }else if(j == 0 && arbre_huffman[i]->occurence < x2 && *x != i){
                x2 = arbre_huffman[i]->occurence;
                *y = i;
            }
        }
        j = 0;
    }
}

/*crée une structure noeud pour chaque caractère contenu dans le fichier*/
void creer_noeud(noeud *tab[], int taille){
    int i, m = 0, n = 0;
    int *x = &m, *y = &n;
    noeud *arbre_huffman;
    for(i=0;i<taille-1;i++){
        arbre_huffman = (noeud*)malloc(sizeof(noeud));
        if(arbre_huffman == NULL){
            printf("Erreur allocation mémoire.\n");
            return;
        }
        parcours_tableau(tab, taille, x, y);
        arbre_huffman->caractere = '\0';
        arbre_huffman->occurence = tab[*x]->occurence + tab[*y]->occurence;
        arbre_huffman->codage[0] = '\0';
        arbre_huffman->nb_bits = 0;
        arbre_huffman->gauche = tab[*x];
        arbre_huffman->droit = tab[*y];
        tab[*x] = arbre_huffman;
        tab[*y] = NULL;
    }
    tab[0] = tab[*x];
    if(*x != 0){
        tab[*x] = NULL;
    }
}

/*retourne vrai si noeud_courant est une feuille*/
int est_feuille(noeud *noeud_courant){
    if(noeud_courant->caractere != '\0'){
        return 1;
    }
    return 0;
}

void creer_code(noeud *noeud_courant, noeud *retour[256], int code, int profondeur){
    int i = 0;
    char code_char[32] = {0}, tmp[32] = {0}, tmp2[32] = {0};
    if(est_feuille(noeud_courant)){
        while(retour[i] != NULL){
            i++;
        }
        retour[i] = (noeud*)malloc(sizeof(noeud));
        if(retour[i] == NULL){
            printf("Erreur allocation mémoire.\n");
            return;
        }
        retour[i]->caractere = noeud_courant->caractere;
        retour[i]->occurence = noeud_courant->occurence;
        sprintf(code_char, "%d", code);
        while((int)strlen(code_char) < profondeur){
            tmp[0] = '0';
            tmp[1] = '\0';
            strcat(tmp, code_char);
            strcpy(tmp2, tmp);
            strcpy(code_char, tmp2);
        }
        strcpy(retour[i]->codage, code_char);
        retour[i]->nb_bits = profondeur;
        retour[i]->gauche = NULL;
        retour[i]->droit = NULL;
    }else{
        code *= 10;
        profondeur++;
        creer_code(noeud_courant->gauche, retour, code, profondeur);
        code++;
        creer_code(noeud_courant->droit, retour, code, profondeur);
    }
}
