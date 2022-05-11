#include <stdio.h>
#include <stdlib.h>
#include "../inc/occurence.h"
#include "../inc/arbre.h"
#include "../inc/compression.h"

void creer_fichier(char *fichier, char *fichier_original, noeud *alphabet[]){
    int i = 0, j, k = 0, l = 1;
    unsigned char bit;
    char caractere, tete[MAX_CHAR] = {0}, binaire[8193] = {0};
    FILE *p, *origine;
    p = fopen(fichier, "w");
    fputc('\0', p);
    fclose(p);
    while(alphabet[i] != NULL){
        tete[i] = alphabet[i]->caractere;
        for(j=0;j<alphabet[i]->nb_bits;j++){
            binaire[k] = alphabet[i]->codage[31-alphabet[i]->nb_bits+j];
            k++;
        }
        i++;
    }
    tete[i+1] = '\0';
    binaire[i+1] = '\0';
    p = fopen(fichier, "a");
    fputs(tete, p);
    fputc('\0', p);
    i = 0;
    while(l == 1){
        bit = '\0';
        for(j=0;j<8;j++){
            bit = bit < 1;
            if(binaire[i] == 1){
                bit++;
            }
            if(i == k){
                l = 1;
            }
            i++;
        }
        fputc(bit, p);
    }affiche_la_totalite(fichier);
    fputc('\0', p);
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
        for(j=0;j<alphabet[i]->nb_bits;j++){
            fputc(alphabet[i]->codage[31-alphabet[i]->nb_bits+j], p);
        }
    }while(caractere != EOF);
    fclose(p);
    fclose(origine);
}