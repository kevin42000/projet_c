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

int main_compression(int argc, char *argv[]){
    int i, taille = 0, existe = 0;
    int tab[MAX_CHAR] = {0};
    char code[MAX_PROF] = {0}, nom_fichier[MAX_CHAR] = {0};
    noeud *arbre_huffman[MAX_CHAR] = {0}, *alphabet[MAX_CHAR] = {0};
    FILE *fichier;
    if(argc !=4){
        printf("Veuillez entrer les arguments au format -c archive_finale fichier_à_compresser.\n");
        return 0;
    }
    if(access(argv[2], F_OK)==0){
        printf("%s existe déjà.\n", argv[2]);
        existe = 1;
    }
    if(access(argv[3], F_OK)!=0){
        printf("%s n'existe pas.\n", argv[3]);
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
    code[0] = '0';
    code[1] = '\0';
    creer_code(arbre_huffman[0], alphabet, code, 1);
    strcpy(nom_fichier, change_nom_fichier(argv[2]));
    if(existe == 1){
        printf("Nouveau nom de l'archive: %s\n", nom_fichier);
    }
    creer_fichier(nom_fichier, argv[3], alphabet);
    for(i=0;i<MAX_CHAR;i++){
        free(alphabet[i]);
    }
    return 1;
}

int main_decompression(int argc, char *argv[]){/* verifications pre-decompression */
    char *p = NULL, fichier[MAX_CHAR] = {0}, nom_fichier[MAX_CHAR] = {0};
    noeud *alphabet[MAX_CHAR] = {0};
    DIR *repertoire;
    
    if(argc !=3 && argc !=4){
        printf("Veuillez entrer les arguments au format -d archive_a_décompresser dossier_cible.\n");
        return 0;
    }
    if(access(argv[2], F_OK)!=0){
        printf("%s n'existe pas.\n",argv[2]);/* je pense que c'est mieux de donner le chemin plutôt que la variable du manuelle */
        return 0;
    }
    decompression(alphabet, argv[2]);
    strcpy(nom_fichier, argv[2]);
    if((p=strrchr(nom_fichier, '.')) && p != NULL && strstr(p, ".huf")){
        strcpy(p, "\0");
    }
    if(argc == 4){
        if((repertoire = opendir(argv[3])) == NULL){
            printf("Création de %s.\n", argv[3]);
            if(access(argv[3], F_OK)==0){
                printf("Impossible de créer le répertoire car un fichier porte déjà le même nom que le répertoire à créer.\n");
                return 0;
            }
            if(mkdir(argv[3], S_IRWXU|S_IRGRP|S_IXGRP) != 0 && access(argv[3], F_OK)!=0){
                printf("Impossible de créer le répertoire.\n");
                return 0;
            }
        }
        closedir(repertoire);
        strcpy(fichier, nom_fichier);
        strcpy(nom_fichier, argv[3]);
        strcat(nom_fichier, "/");
        strcat(nom_fichier, fichier);
    }
    strcpy(nom_fichier, change_nom_fichier(nom_fichier));
    printf("Nom du fichier: %s\n", nom_fichier);
    creation_fichier(alphabet, nom_fichier, argv[2]);
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