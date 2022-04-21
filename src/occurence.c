#include <stdio.h>
#include <stdlib.h>
#include "inc/occurence.h"

void affiche_une_partie(char fichier[]){
  char lecture[100];
  FILE *p;
  p = fopen(fichier, "r");
  fread(lecture, 99, 1, fichier);
  lecture[99] = '\0';
  printf("%s\n", lecture);
  fclose(p);
}

void affiche_la_totalite(char fichier[]){
  char caractere;
  FILE *p;
  p = fopen(fichier, "r");
  do{
    caractere = fgetc(p);
    if(feof(p)){
      break;
    }
    printf'"%c", caractere);
  }while(caractere != EOF);
  fclose(p);
  printf("\n");
}
       
void occurence(FILE *fic, int tab[256]){
  char caractere;
  do{
    caractere = fgetc(p);
    if(feof(p)){
      break;
    }
    tab[caractere]++;
  }while(caractere != EOF);
}
