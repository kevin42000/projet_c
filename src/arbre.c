#include <stdio.h>
#include <stdlib.h>
#include "inc/arbre.h"

noeud *creation_noeud[256](int tab[256]){
	int i = 0;
 	noeud *arbre_huffman[256];
	
	while(tab[i] != 0 && i < 256){
		arbre_huffman[i] = (noeud*)malloc(sizeof(noeud));
		if(arbre_huffman[i] == NULL){
			printf("Erreur allocation mémoire.\n");
			return NULL;
		}
		arbre_huffman[i]->caractere = i;
		arbre_huffman[i]->occurence = tab[i];
		arbre_huffman[i]->codage[0] = '\0';
		arbre_huffman[i]->nb_bits = 0;
		arbre_huffman[i]->gauche = NULL;
		arbre_huffman[i]->droit = NULL;
		i++;
	}
	return arbre_huffman;
}

noeud * creer_feuille(int *tab, int index){
	noeud *arbre_huffman;
	arbre_huffman[index] = (noeud*)malloc(sizeof(noeud));
	if(arbre_huffman[index] == NULL){
		printf("Erreur allocation mémoire.\n");
		return NULL;
	}
	arbre_huffman[index]->caractere = index;
	arbre_huffman[index]->occurence = tab[index];
	arbre_huffman[index]->codage[0] = '\0';
	arbre_huffman[index]->nb_bits = 0;
	arbre_huffman[index]->gauche = NULL;
	arbre_huffman[index]->droit = NULL;
	return arbre_huffman;
}

void parcours_tableau(noeud *arbre_huffman[256], int *x, int *y){
	int i = 0, j = 0;
	*x = 256;
	*y = 256;
	while(arbre_huffman[i] != NULL){
		if(arbre_huffman[i]->occurence < *x){
			j = *x;
			x* = i;
		}
		if(j != 0 && *y < j){
			*y = j;
		}else if(j == 0 && arbre_huffman[i]->occurence < *y){
			y* = i;
		}
		j = 0;
		i++;
	}
}
