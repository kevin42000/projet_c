#include <stdio.h>
#include <stdlib.h>
#include "../inc/occurence.h"
#include "../inc/arbre.h"
#include "../inc/compression.h"

int main(int argc, char *argv[]){
    int i, taille = 0/*, m = 0, n = 0*/;
    /*int *x = &m, *y = &n;*/
    int tab[256] = {0};
    noeud *arbre_huffman[256] = {0}, *alphabet[256] = {0};
    FILE *p;

    if(argc!=2){
        printf("%s\n", argv[0]);
        return 1;
    }
    p = fopen(argv[1], "r");
    occurence(p, tab);
    fclose(p);
    for(i=0;i<256;i++){
        arbre_huffman[taille] = creer_feuille(tab, i);
        if(arbre_huffman[taille] == NULL){
            taille--;
        }
        taille++;
    }
    
    creer_noeud(arbre_huffman, taille);
    
    /*a partir d'ici l'arbre est construit*/
    creer_code(arbre_huffman[0], alphabet, 0, 1);
    i = 0;
    while(alphabet[i] != NULL){
        printf("%c, %d, %s, %d\n", alphabet[i]->caractere, alphabet[i]->occurence, alphabet[i]->codage, alphabet[i]->nb_bits);
        i++;
    }
    printf("\n");
    creer_fichier("exemple.huf", argv[1], alphabet);

    for(i=0;i<256;i++){
        free(alphabet[i]);
    }
    
    decompression(alphabet, "exemple.huf");
    i = 0;
    while(alphabet[i] != NULL){
        printf("%c, %d, %s, %d\n", alphabet[i]->caractere, alphabet[i]->occurence, alphabet[i]->codage, alphabet[i]->nb_bits);
        i++;
    }
    printf("\n");

    creation_fichier(alphabet, "exemple2.txt", "exemple.huf");
    /*if(argc == 1){
    
  }else if(argv[1] == "-c"){
    
  }else if(argv[1] == "-d"){
    
  }else if(argv[1] == "-h"){
    
  }*/
  exit(EXIT_SUCCESS);
}
