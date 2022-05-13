#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <unistd.h>
#include "../inc/occurence.h"
#include "../inc/arbre.h"
#include "../inc/compression.h"

int main_compression(int argc, char *argv[]){
    int i, taille = 0, existe = 0;
    int tab[MAX_CHAR] = {0};
    char ajout_nom[4] = {0}, nom_fichier[MAX_CHAR] = {0};
    noeud *arbre_huffman[MAX_CHAR] = {0}, *alphabet[MAX_CHAR] = {0};
    FILE *fichier;
    if(argc !=4){
        printf("Veuillez entrer les arguments au format -c archive_finale dossiers_ou_fichiers_a_compresser.\n");
        return 0;
    }
    if(access(argv[2], F_OK)==0){
        printf("<archive_finale> existe déjà.\n");
        existe = 1;
    }
    if(access(argv[3], F_OK)!=0){
        printf("<dossiers_ou_fichiers_à_compresser> n'existe pas.\n");
        return 0;
    }
    fichier = fopen(argv[3], "r");
    occurence(fichier, tab);
    fclose(fichier);
    for(i=0;i<MAX_CHAR;i++){
        arbre_huffman[taille] = creer_feuille(tab, i);
        if(arbre_huffman[taille] == NULL){
            taille--;
        }
        taille++;
    }
    creer_noeud(arbre_huffman, taille);
    creer_code(arbre_huffman[0], alphabet, 0, 1);
    if(existe == 0){
        creer_fichier(argv[2], argv[3], alphabet);
    }else{
        while(access(nom_fichier, F_OK)==0 && existe < INT_MAX){
            strcpy(ajout_nom, "(");
            strcat(ajout_nom, existe+'0');
            strcat(ajout_nom, ")");
            strcpy(nom_fichier, argv[2]);
            strcat(nom_fichier, ajout_nom);
            existe++;
        }
        if(existe == INT_MAX){
            printf("Impossible de créer l'archive.\n");
            return 0;
        }
    }
    for(i=0;i<MAX_CHAR;i++){
        free(alphabet[i]);
    }
    return 1;
}

int main_decompression(int argc, char *argv[]){
    int arg = 0;
    char fichier[MAX_CHAR] = {0};
    noeud *alphabet[MAX_CHAR] = {0};
    if(argc !=3 && argc !=4){
        printf("Veuillez entrer les arguments au format -d archive_a_décompresser dossier_cible.\n");
        return 0;
    }
    if(access(argv[2], F_OK)!=0){
        printf("<archive_à_décompresser> n'existe pas.\n");
        return 0;
    }
    if(argc == 4 && access(argv[3], F_OK)==0){
        printf("<dossier_cible> existe déjà.\n");
        arg = 1;
        return 0;
    }
    decompression(alphabet, argv[2]);
    if(arg == 1){
        creation_fichier(alphabet, argv[3], argv[2]);
    }else{
        strcpy(fichier, argv[2]);
        strcat(fichier, ".txt");
        if(access(fichier, F_OK)!=0){
            creation_fichier(alphabet, fichier, argv[2]);
        }else{
            printf("Impossible de créer <dossier_cible>=%s.\n", fichier);
            return 0;
        }
    }
    return 1;
}

void help(){
    printf("\n\nManuel utilisateur :\n\n");
    printf("Pour compresser un fichier, veuillez utiliser le logiciel avec la commande :\n");
    printf("./<logiciel> -c archive_finale dossiers_ou_fichiers_à_compresser.\n\n");
    printf("Pour décompresser un fichier, veuillez utiliser le logiciel avec la commande :\n");
    printf("./<logiciel> -d archive_à_décompresser dossier_cible.\n");
    printf("L'option dossier_cible est facultative\n\n");
    printf("Pour afficher l'aide, veuillez entrer la commande :\n");
    printf("./<logiciel> -h\n\n");
}

int main(int argc, char *argv[]){
    int option;
    if((option = getopt(argc, argv, "cdh")) != -1){
        switch(option){
        case('c'):
            main_compression(argc, argv);
            break;
        case('d'):
            main_decompression(argc, argv);
            break;
        case('h'):
            help();
            break;
        default:
            printf("Veuillez utiliser l'option -h pour voir le guide d'utilisation du logiciel.\n");
        }
    }else{
        printf("Veuillez utiliser l'option -h pour voir le guide d'utilisation du logiciel.\n");
    }
    return 1;
}