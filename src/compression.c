#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../inc/occurence.h"
#include "../inc/arbre.h"
#include "../inc/compression.h"
#include "../inc/pile.h"
void afficher_octet(char b){
    int i;
    for(i=0;i<8;i++){
        printf("%d",!!((b << i) & 0x80));
        /* 
        0x80 = 128 (l'hexa est plus stylé je trouve)
        on prend l'inverse de l'inverse du du décalage de i en operant un AND avec 128 (1000 0000)
        on affiche donc l'octet voulut
        */
    }
    printf("\n");
}
void creer_fichier(char *fichier, char *fichier_original, noeud *alphabet[]){
    int i = 0, j, k = 0,m,x;
    unsigned char bit;
    char nb[11], nb_bits[11], caractere, tete[45] = {0};
    FILE *p, *origine;
    pile pile = pile_vide(); /* pour stocker les bits */
    
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
    /* début de l'écriture du fichier binaire */
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
        bit = 0;
        for(j=0;j<alphabet[i]->nb_bits;j++){
            if(alphabet[i]->codage[j] == '1'){
                pile = empiler(pile,1);
            }else{
                pile = empiler(pile,0);
            }
            if(taille_pile(pile) == 8){
                bit = 0;
                for(k=0;k<8;k++){
                    if((x=sommet(pile)) == 0){
                        bit <<=1;
                    }else{
                        bit = (bit << 1) + 1;
                    }
                    pile = depiler(pile);
                }
                fwrite(&bit,sizeof(char),1,p);
                bit=0;
            }
        }
    }while(caractere != EOF);
    if(taille_pile(pile) > 0){
            m = 8 - taille_pile(pile);
            while(!est_pile_vide(pile)){
                if(sommet(pile) == 0){
                        bit <<=1;
                    }else{
                        bit = (bit << 1) + 1;
                    }
                    pile = depiler(pile);
            }
            for(j=0;j<m;j++){
                bit <<=1;
            }
            fwrite(&bit,sizeof(char),1,p);
            bit=0;
        }
    fclose(p);
    fclose(origine);
}

void decompression(noeud *alphabet[MAX_CHAR], char *fichier){
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

void creation_fichier(noeud *alphabet[MAX_CHAR], char *fichier, char *fichier_compresse){
    FILE *comp,*dest;
    int ignore = 0;
    char c;
    puts("Done reading");
    comp = fopen(fichier_compresse,"r");
    dest = fopen(fichier,"w");
    if(comp == NULL || dest == NULL){
        mon_erreur("Erreur de fichier\n");
    }
    while(fgetc(comp) != '\n'){
        ignore++;
    }
    fclose(comp);
    comp = fopen(fichier_compresse,"rb");
    if(comp == NULL){
        mon_erreur("Impossible d'ouvrir le compressé !\n");
    }
    fseek(comp,0,SEEK_SET);
    fread(&c,sizeof(char),ignore,comp);
    printf("LOL\n");
}