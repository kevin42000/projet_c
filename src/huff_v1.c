#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <ctype.h>
#include <unistd.h>
#include <getopt.h>
#include <dirent.h>
#include <sys/stat.h>
#include "../inc/occurence.h"
#include "../inc/arbre.h"
#include "../inc/compression.h"
#include "../inc/fonction_huff.h"

int main_compression_v0(int argc, char *argv[]){
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
        printf("<archive_finale> existe déjà.\n");
        existe = 1;
    }
    if(access(argv[3], F_OK)!=0){
        printf("<fichier_à_compresser> n'existe pas.\n");
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

int main_compression(int argc, char *argv[]){
    int i, j;
    char caractere, nb[11] = {0}, *chaine, nom_fichier[MAX_CHAR] = {0};
    char *argv2[4] = {0}, **liste_avant, **liste_tmp;
    FILE *fichier, *tmp;
    if(argc < 4){
        printf("Veuillez entrer les arguments au format -c archive_finale fichiers_à_compresser.\n");
        return 0;
    }
    if(access(argv[2], F_OK)==0){
        printf("%s existe déjà.\n", argv[2]);
    }
    for(i=3;i<argc;i++){
        if(access(argv[i], F_OK)!=0){
            printf("%s n'existe pas.\n", argv[i]);
            return 0;
        }
    }
    
    liste_avant = liste_repertoire("./");
    argv2[0] = NULL;
    argv2[1] = NULL;
    argv2[2] = (char*)calloc(MAX_CHAR, sizeof(char));
    strcpy(argv2[2], "tmp");
    argv2[3] = (char*)calloc(MAX_CHAR, sizeof(char));
    for(i=3;i<argc;i++){
        strcpy(argv2[3], argv[i]);
        main_compression_v0(4, argv2);
    }
    liste_tmp = liste_fichier_tmp("./", liste_avant);
    /*maintenant, on a la liste des fichiers temporaires et tous les fichiers ont étés compressés dans ces fichiers temporaires.*/
    strcpy(nom_fichier, change_nom_fichier(argv[2]));
    printf("Nom de l'archive: %s\n", nom_fichier);
    fichier = fopen(nom_fichier, "w");
    sprintf(nb, "%d", argc-3);
    fputs(nb, fichier);
    fputc('\n', fichier);
    for(i=3;i<argc;i++){
        if((chaine = strrchr(argv[i], '/')) != NULL){
            fputs((chaine + 1), fichier);
        }else{
            fputs(argv[i], fichier);
        }
        fputc('\n', fichier);
    }
    
    for(i=0;i<argc-3;i++){
        j = 1;
        tmp = fopen(liste_tmp[i], "r");
        do{
            caractere = fgetc(tmp);
            if(feof(tmp)){
                break;
            }else if(caractere == '\n'){
                j++;
            }
        }while(caractere != EOF);
        fclose(tmp);
        sprintf(nb, "%d", j);
        fputs(nb, fichier);
        fputc('\n', fichier);
        tmp = fopen(liste_tmp[i], "r");
        do{
            caractere = fgetc(tmp);
            if(feof(tmp)){
                break;
            }
            fputc(caractere, fichier);
        }while(caractere != EOF);
        fclose(tmp);
        fputc('\n', fichier);
        remove(liste_tmp[i]);
    }
    fclose(fichier);
    return 1;
}

int main_decompression_v0(int argc, char *argv[]){/* verifications pre-decompression */
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

int main_decompression(int argc, char *argv[]){
    int i, j, taille;
    char *p = NULL, nb[11] = {0}, nom[4097] = {0}, contenu, nom_fichier[MAX_CHAR] = {0};
    char **liste_tmp, **liste_origine;
    FILE *fichier, *tmp;
    DIR *repertoire;
    if(argc !=3 && argc !=4){
        printf("Veuillez entrer les arguments au format -d archive_a_décompresser dossier_cible.\n");
        return 0;
    }
    if(access(argv[2], F_OK)!=0){
        printf("<archive_à_décompresser> n'existe pas.\n");
        return 0;
    }printf("1\n");
    fichier = fopen(argv[2], "r");printf("1\n");
    if(fscanf(fichier, "%s", nb) != EOF){printf("1\n");
        if(!isdigit(nb[0])){printf("1\n");
            fclose(fichier);
            return main_decompression_v0(argc, argv);
        }
    }else{
        fclose(fichier);
        return 0;
    }printf("1\n");
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
    }
    liste_origine = creer_liste(atoi(nb));
    liste_tmp = creer_liste(atoi(nb));
    for(i=0;i<atoi(nb);i++){
        if(fscanf(fichier, "%s", nom) != EOF){
            strcpy(liste_origine[i], nom);
        }else{
            return 0;
        }
    }
    
    for(i=0;i<atoi(nb);i++){
        if(fscanf(fichier, "%d", &taille)){
            j = 0;
            fgetc(fichier);
            strcpy(liste_tmp[i], "tmp");
            strcpy(liste_tmp[i], change_nom_fichier(liste_tmp[i]));
            tmp = fopen(liste_tmp[i], "w");
            while(j < taille){
                contenu = fgetc(fichier);
                fputc(contenu, tmp);
                if(contenu == '\n'){
                    j++;
                }
            }
            fclose(tmp);
        }else{
            return 0;
        }
    }
    fclose(fichier);
    
    for(i=0;i<atoi(nb);i++){
        noeud *alphabet[MAX_CHAR] = {0};
        decompression(alphabet, liste_tmp[i]);
        if((p=strrchr(liste_tmp[i], '.')) && p != NULL && strstr(p, ".huf")){
            strcpy(p, "\0");
        }

        if(argc == 3){
            strcpy(liste_origine[i], change_nom_fichier(liste_origine[i]));
            creation_fichier(alphabet, liste_origine[i], liste_tmp[i]);
        }else{
            strcpy(nom_fichier, argv[3]);
            strcat(nom_fichier, "/");
            strcat(nom_fichier, liste_origine[i]);
            strcpy(nom_fichier, change_nom_fichier(nom_fichier));
            creation_fichier(alphabet, nom_fichier, liste_tmp[i]);
        }
        remove(liste_tmp[i]);
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