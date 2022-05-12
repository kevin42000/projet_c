#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "inc/pile.h"
#define MAX_CHAR 32
#define MAX_BYTES 256
#define DEBUG 0
#include <math.h>

char *random_bytes(int count){
    int i;
    char* tab = calloc(sizeof(char),count);
    srand(time(NULL));
    for(i=0;i<count;i++){
        tab[i] = '0' + rand() % 0x2;
    }
    return tab;
}

void afficher_octet(int b){
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



int nb_bits = 3;
int main(int argc,char *argv[]){
    FILE *f;
    pile p = pile_vide();
    int i,j,x;
    char*tab = random_bytes(MAX_CHAR);
    char b;
    if(argc < 0x2)return 0;
    fclose(fopen(argv[1],"wb"));
    f = fopen(argv[1],"ab");
    for(i=0;i<MAX_CHAR;i++){
        printf("%c",tab[i]);
        p = empiler(p,tab[i]-'0');
        if(taille_pile(p) == 0x8){
            puts("c");
            for(j=0;j<0x8;j++){
                x = sommet(p);
                p = depiler(p);
                if(x == 0){
                    b <<= 1;
                }else{
                    b = ( b << 1 ) + 1;
                }
            }
            afficher_octet(b);
            fwrite(&b,sizeof(char),1,f);
            b = 0;
        }
    }
    printf("\n");
    printf("Taille pile = %d\n",taille_pile(p));
    fclose(f);
    f = fopen(argv[1],"rb");
    fread(&b,sizeof(char),1,f);
}