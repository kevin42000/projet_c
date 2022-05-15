#include <stdio.h>
#include <stdlib.h>
#include "../inc/occurence.h"

void affiche_une_partie(char *fichier){
  char lecture[10];
  FILE *p;
  p = fopen(fichier, "r");
  fread(lecture, 9, 1, p);
  lecture[9] = '\0';
  printf("%s\n", lecture);
  fclose(p);
}


void affiche_la_totalite(char *fichier){
  char caractere;
  FILE *p;
  p = fopen(fichier, "r");
  do{
    caractere = fgetc(p);
    if(feof(p)){
      break;
    }
  printf("%c", caractere);
  }while(caractere != EOF);
  fclose(p);
  printf("\n");
}
void occurence(FILE *fic, int tab[MAX_CHAR]){
  char caractere; /* vaut mieux éviter le unsigned char */
  while(1){
    caractere = fgetc(fic);
    if(feof(fic)){
      break;
    }
    tab[(unsigned int)caractere]++;
  }
}
