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
void parcours_tableau(noeud *arbre_huffman[MAX_CHAR], int taille, int *x, int *y){
    int i = 0, j = 0;
    int x1 = INT_MAX, x2 = INT_MAX;
    *x = MAX_CHAR;
    *y = MAX_CHAR;
    for(i=0;i<taille;i++){
        if(arbre_huffman[i] != NULL){
            if(arbre_huffman[i]->occurence < x1){
                j = *x;
                x1 = arbre_huffman[i]->occurence;
                *x = i;
            }
            if(j > 0 && j < MAX_CHAR && arbre_huffman[j]->occurence < x2){
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

void creer_code(noeud *noeud_courant, noeud *retour[MAX_CHAR], char code[33], int profondeur){
    int i = 0;
    char code_char[32] = {0}, tmp[33] = {0};
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
        strcpy(retour[i]->codage, code);
        retour[i]->nb_bits = profondeur;
        retour[i]->gauche = NULL;
        retour[i]->droit = NULL;
    }else{
        tmp[0] = '0';
        tmp[1] = '\0';
        strcpy(code_char, code);
        strcat(code_char, tmp);
        profondeur++;
        creer_code(noeud_courant->gauche, retour, code_char, profondeur);
        tmp[0] = '1';
        tmp[1] = '\0';
        strcpy(code_char, code);
        strcat(code_char, tmp);
        creer_code(noeud_courant->droit, retour, code_char, profondeur);
    }
}
