#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../inc/occurence.h"
#include "../inc/arbre.h"
#include "../inc/compression.h"

void creer_fichier(char *fichier, char *fichier_original, noeud *alphabet[]){
    int i = 0, j, k = 0;
    unsigned char bit;
    char nb[11], nb_bits[11], caractere, tete[45] = {0};
    FILE *p, *origine;
    
    while(alphabet[i] != NULL){
        i++;
    }
    p = fopen(fichier, "w");
    sprintf(nb, "%d", i+1);
    fputs(nb, p);
    fclose(p);
    i = 0;
    p = fopen(fichier, "a");
    fputc(' ', p);
    
    while(alphabet[i] != NULL){
        tete[0] = alphabet[i]->caractere;
        tete[1] = '\0';
        if(alphabet[i]->nb_bits > 9){
            strcat(tete, "1\0");
        }else{
            strcat(tete, "0\0");
        }
        sprintf(nb_bits, "%d", alphabet[i]->nb_bits);
        strcat(tete, nb_bits);
        strcat(tete, alphabet[i]->codage);
        fputs(tete, p);
        i++;
    }
    
    fputc('\n', p);
    fclose(p);
    
    p = fopen(fichier, "ab");
    origine = fopen(fichier_original, "r");
    do{
        caractere = fgetc(origine);
        if(feof(origine)){
            break;
        }
        i = 0;
        while(alphabet[i]->caractere != caractere){
            i++;
        }
        bit = '\0';
        for(j=0;j<alphabet[i]->nb_bits;j++){
            if(alphabet[i]->codage[31-alphabet[i]->nb_bits+j] == 1){
                bit++;
            }
            bit = bit < 1;
            k++;
            if(k == 8){
                fputc(bit, p);
                bit = '\0';
                k = 0;
            }
        }
    }while(caractere != EOF);
    fclose(p);
    fclose(origine);
}

void decompression(noeud *alphabet[256], char *fichier){
    int i = 0, j, nb;
    char nb_bits[3], taille_arbre[11] = {0};
    FILE *p;
    p = fopen(fichier, "r");
    while((taille_arbre[i] = fgetc(p)) != ' '){
        i++;
    }
    taille_arbre[i] = '\0';
    nb = atoi(taille_arbre);
    
    for(i=0;i<nb-1;i++){
        alphabet[i] = (noeud*)malloc(sizeof(noeud));
        if(alphabet[i] == NULL){
            printf("Erreur allocation mémoire.\n");
            return;
        }
        alphabet[i]->caractere = fgetc(p);
        alphabet[i]->occurence = 0;
        if((nb_bits[0] = fgetc(p)) == '0'){
            nb_bits[0] = fgetc(p);
            nb_bits[1] = '\0';
        }else{
            nb_bits[0] = fgetc(p);
            nb_bits[1] = fgetc(p);
            nb_bits[2] = '\0';
        }
        alphabet[i]->nb_bits = atoi(nb_bits);
        for(j=0;j<alphabet[i]->nb_bits;j++){
            alphabet[i]->codage[j] = fgetc(p);
        }
        alphabet[i]->codage[j+1] = '\0';
        alphabet[i]->gauche = NULL;
        alphabet[i]->droit = NULL;
    }
    fclose(p);
}

void creation_fichier(noeud *alphabet[256], char *fichier, char *fichier_compresse){
    
}