#include <stdio.h>
#include <stdlib.h>
#include "../inc/occurence.h"
#include "../inc/arbre.h"

int main(int argc, char *argv[]){
    int i, m = 0, n = 0;
    int *x = &m, *y = &n;
    int tab[256] = {0};
    noeud *arbre_huffman[256];
    FILE *p;

    if(argc!=2){
        return 1;
    }
    p = fopen(argv[1], "r");
    occurence(p, tab);
    fclose(p);
    /*affiche_une_partie(argv[1]);
      affiche_la_totalite(argv[1]);
    p = fopen(argv[1], "r");
    occurence(p, tab);
    fclose(p);
    for(i=0;i<256;i++){
        printf("%d ", tab[i]);
    }
    printf("\n");*/
    for(i=0;i<256;i++){
        arbre_huffman[i] = creer_feuille(tab, i);/*printf("%c ", arbre_huffman[i]->caractere);*/
    }
    /*printf("\n");*/
    
    parcours_tableau(arbre_huffman, x, y);
    printf("%d %d\n", *x, *y);

    creer_noeud(arbre_huffman, 256);
    printf("%d %d\n", arbre_huffman[0]->occurence, arbre_huffman[0]->gauche->occurence);
    /*if(argc == 1){
    
  }else if(argv[1] == "-c"){
    
  }else if(argv[1] == "-d"){
    
  }else if(argv[1] == "-h"){
    
  }*/
  exit(EXIT_SUCCESS);
}
