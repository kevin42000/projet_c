#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <unistd.h>
#include <getopt.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include "../inc/occurence.h"
#include "../inc/arbre.h"
#include "../inc/compression.h"
#include "../inc/fonction_huff.h"

char * change_nom_fichier(char ancien_nom[MAX_CHAR]){
    int i = 0;
    char *nouveau_nom, c[11] = {0};
    nouveau_nom = (char*)calloc(MAX_CHAR, sizeof(char));
    if(nouveau_nom == NULL){
        printf("Erreur allocation mémoire.");
        return NULL;
    }
    strcpy(nouveau_nom, ancien_nom);
    while(access(nouveau_nom, F_OK)==0 && i < INT_MAX){
        strcpy(nouveau_nom, ancien_nom);
        sprintf(c, "%d", i);
        strcat(nouveau_nom, c);
        i++;
    }
    if(i == INT_MAX){
        printf("Impossible de créer l'archive.\n");
        return NULL;
    }
    return nouveau_nom;
}

char ** creer_liste(int taille){
    int i;
    char **liste;
    liste = (char**)malloc(taille * sizeof(char*));
    if(liste == NULL){
        printf("Erreur allocation mémoire.");
        return NULL;
    }
    for(i=0;i<taille;i++){
        liste[i] = (char*)calloc(MAX_CHAR, sizeof(char));
        if(liste[i] == NULL){
            printf("Erreur allocation mémoire.");
            return NULL;
        }
    }
    return liste;
}

char ** liste_repertoire(char chemin[MAX_CHEMIN]){
    int i = 0;
    char **fichier;
    DIR *repertoire;
    struct dirent *liste;
    if((repertoire = opendir(chemin)) == NULL){
        printf("Le répertoire %s n'existe pas.\n", chemin);
        return NULL;
    }
    while((liste = readdir(repertoire))){
        if(liste->d_type == 8){
            i++;
        }
    }
    closedir(repertoire);
    fichier = creer_liste(i);
    i = 0;
    if((repertoire = opendir(chemin)) == NULL){
        printf("Le répertoire %s n'existe pas.\n", chemin);
        return NULL;
    }
    while((liste = readdir(repertoire))){
        if(liste->d_type == 8){
            strcpy(fichier[i], liste->d_name);
            i++;
        }
    }
    closedir(repertoire);
    return fichier;
}

char ** liste_fichier_tmp(char chemin[MAX_CHEMIN], char **liste){
    int i = 0, j = 0, k, l, m, n = 0;
    char **liste_tmp, **liste_fichier;
    liste_fichier = liste_repertoire(chemin);
    while(liste[i] != NULL){
        i++;
    }
    while(liste_fichier[j] != NULL){
        j++;
    }
    liste_tmp = creer_liste(j-i);
    for(k=0;k<i;k++){
        m = 0;
        for(l=0;l<j;l++){
            if(strcmp(liste[k], liste_fichier[l]) == 0){
                m = 1;
                break;
            }
        }
        if(m == 0){
            strcpy(liste_tmp[n], liste[k]);
            n++;
        }
    }
    return liste_tmp;
}