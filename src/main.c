#include <stdio.h>
#include <stdlib.h>
#include "../inc/occurence.h"
#include "../inc/arbre.h"

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
    /*alphabet contient le codage et la profondeur. Le codage affiché commençant forcément par un 1, on peut savoir si un élément commence par 0 grâce à la profondeur. Par ex pour x, le codage est 0 et a une profondeur de 7, le code binaire est donc 0000000 et pour c le code binaire est 0000001.*/
    /*if(argc == 1){
    
  }else if(argv[1] == "-c"){
    
  }else if(argv[1] == "-d"){
    
  }else if(argv[1] == "-h"){
    
  }*/
  exit(EXIT_SUCCESS);
}
