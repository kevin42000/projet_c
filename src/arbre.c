#include <stdio.h>
#include <stdlib.h>
#include "../inc/arbre.h"

noeud * creer_feuille(int *tab, int index){
    noeud *arbre_huffman;
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

void parcours_tableau(noeud *arbre_huffman[256], int *x, int *y){
    int i = 0, j = 0;
    *x = 256;
    *y = 256;
    while(arbre_huffman[i] != NULL){
        if(arbre_huffman[i]->occurence < *x){
            j = *x;
            *x = i;
        }
        if(j != 0 && *y < j){
            *y = j;
        }else if(j == 0 && arbre_huffman[i]->occurence < *y){
            *y = i;
        }
        j = 0;
        i++;
    }
}

void creer_noeud(noeud *tab[], int taille){
    int m = 0, n = 0;
    int *x = &m, *y = &n;noeud *arbre_huffman;
    arbre_huffman = (noeud*)malloc(sizeof(noeud));
    if(arbre_huffman == NULL){
        printf("Erreur allocation mémoire.\n");
        return;
    }
    parcours_tableau(tab, x, y);
    arbre_huffman->caractere = '\0';
    arbre_huffman->occurence = tab[*x]->occurence + tab[*y]->occurence;
    arbre_huffman->codage[0] = '\0';
    arbre_huffman->nb_bits = 0;
    arbre_huffman->gauche = tab[*x];
    arbre_huffman->droit = tab[*y];
    tab[*x] = arbre_huffman;
    tab[*y] = NULL;
}