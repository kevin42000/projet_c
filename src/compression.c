#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../inc/occurence.h"
#include "../inc/arbre.h"
#include "../inc/compression.h"
#include "../inc/pile.h"
#include "../inc/file.h"

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
    unsigned long int tot = 0;
    unsigned char bit;
    char nb[11], nb_bits[11], caractere, tete[45] = {0};
    FILE *p, *origine;
    file f = file_vide(); /* pour stocker les bits */
    i=0;
    while(alphabet[i] != NULL){
        printf("%c %s %d\n",alphabet[i]->caractere,alphabet[i]->codage,alphabet[i]->nb_bits);
        i++;
    }
    origine = fopen(fichier_original, "r");
    fseek(origine, 0L, SEEK_END);
    tot = ftell(origine);
    p = fopen(fichier, "w");
    fprintf(p,"");
    fclose(p);
    p = fopen(fichier, "a");
    i = 0;
    while(alphabet[i] != NULL){
        fprintf(p,"%c %ld;;",alphabet[i]->caractere,alphabet[i]->occurence);
        i++;
    }
    fputc('\0', p);
    fclose(p);
    /* début de l'écriture du fichier binaire */
    p = fopen(fichier, "ab");
    fseek(p,0,SEEK_SET);
    fseek(origine,0,SEEK_SET);
    do{
        caractere = fgetc(origine);
        if(feof(origine)){
            puts("Done building");
            break;
        }
        i = 0;
        while(alphabet[i]->caractere != caractere){
            i++;
        }
        bit = 0;
        for(j=0;j<alphabet[i]->nb_bits;j++){
            if(alphabet[i]->codage[j] == '1'){
                f = enfiler(f,1);
            }else{
                f = enfiler(f,0);
            }
            if(taille_file(f) == 8){
                bit = 0;
                for(k=0;k<8;k++){
                    if((x=debut_file(f)) == 0){
                        bit <<=1;
                    }else{
                        bit = (bit << 1) + 1;
                    }
                    f = defiler(f);
                }
                fwrite(&bit,sizeof(char),1,p);
                bit=0;
            }
        }
    }while(caractere != EOF);
    if(taille_file(f) > 0){
            m = 8 - taille_file(f);
            while(!est_file_vide(f)){
                if(debut_file(f) == 0){
                        bit <<=1;
                    }else{
                        bit = (bit << 1) + 1;
                    }
                    f = defiler(f);
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
    int i = 0, j,taille = 0;
    char nb_bits[3],c;
    int tab[MAX_CHAR],nbr;
    char code[MAX_PROF] = {0};
    noeud *tmp[MAX_CHAR];
    FILE *p;
    p = fopen(fichier, "r");
    for(i = 0;i<MAX_CHAR;i++){
        tab[i] = 0;
    }
    while(fscanf(p,"%c %d;;",&c,&nbr) == 2){
        tab[c] = nbr;
    }
    fclose(p);
    for(i=0;i<MAX_CHAR;i++){
        tmp[taille] = creer_feuille(tab, i);
        if(tmp[taille] == NULL){
            taille--;
        }
        taille++;
    }
    creer_noeud(tmp, taille);
    code[0] = '0';
    code[1] = '\0';
    creer_code(tmp[0], alphabet, code, 1);
    i=0;
    while(i < MAX_CHAR && alphabet[i] != NULL){
        /*printf("%c %s %d\n",alphabet[i]->caractere,alphabet[i]->codage,alphabet[i]->nb_bits);*/
        i++;
    }
    /* Libération de tmp */
    for(i=0;tmp[i] != NULL;i++){
            free(tmp[i]);
    }
}

void print_r(unsigned int seq[MAX_PROF]){
    int i =0;
    while(seq[i] != -1){
        printf("%d",seq[i]);
        i++;
    }
    printf("\n");
}

int get_char(noeud *alphabet[MAX_CHAR],unsigned int seq[MAX_PROF]){
    int i,j,state = 1,count = 0,len;
    for(len = 0;seq[len] != -1;len++);
    for(i=0;i<MAX_CHAR;i++){
        state = 1;
        if(alphabet[i] == NULL)return -1;/* On est arrivé à la fin */
        if(alphabet[i]->nb_bits != len){
            continue;
        }
        count = 0;
        for(j=0;j<alphabet[i]->nb_bits;j++){
            if(seq[j] == -1){
                state = 0;
                break;
            }
            if(alphabet[i]->codage[j] == seq[j]+'0'){
            }else{
                state = 0;
                break;
            }
        }
        if(state == 1){
            return alphabet[i]->caractere;
        }
    }
    return -1;
}

void creation_fichier(noeud *alphabet[MAX_CHAR], char *fichier, char *fichier_compresse){
    FILE *comp,*dest;
    int ignore = 0;
    float percent = 0;
    char c;
    char progress[13];
    unsigned long int size =0,max_size=0;
    unsigned int seq[MAX_PROF] = {-1};
    unsigned int t;
    char decimal[4];
    int i = 0, j, nb;
    char nb_bits[3], taille_arbre[11] = {0};
    FILE *p;
    file f = file_vide();
    p = fopen(fichier_compresse, "rb");
    dest = fopen(fichier,"w");
    fseek(p,0,SEEK_SET);
    while(fscanf(p,"%c %ld;;",&c,&t) == 2){
        size += t;
    }
    max_size = size;
    fseek(p,0,SEEK_SET);
    while(fgetc(p) != '\0');
    t = 0;
    c = 0;
    while((!feof(p) || !est_file_vide(f)) && size > 0){
        if(!feof(p)){
            if(taille_file(f) < 32){
            fread(&c,sizeof(char),1,p);
            for(i=7;i>=0;i--){
                t = ((c >> i) & 0x01);/* On récupère le dernier bit de la série après un décalage */
                f = enfiler(f,t);/* On l'enfile*/
            }
            }
        }
        while(!est_file_vide(f)){
            t = debut_file(f);
            f= defiler(f);
            for(j=0;seq[j] != -1;j++);
            seq[j] = t;
            seq[j+1] = -1;
            c=get_char(alphabet,seq);
            if(c != -1){
                break;
            }
        }
        if(c != -1){
            seq[0] = -1;
        }
        fprintf(dest,"%c",c);
        size--;
        percent = ((double)(max_size-size)/(double)max_size)*100.0;
        progress[0] = '[';
        for(i=1;i<=10;i++){
            progress[i] = (10.0*i-1) < percent ? '*' : '-';
        }
        progress[11] = ']';
        progress[12] = '\0';
        printf("\r%.0f%% %s",percent,progress);
        fflush(stdout);
    }
    printf("\n");
    return;
}