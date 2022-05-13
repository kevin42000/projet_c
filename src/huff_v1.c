#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <unistd.h>
#include <getopt.h>
#include <dirent.h>
#include <sys/stat.h>
#include "../inc/occurence.h"
#include "../inc/arbre.h"
#include "../inc/compression.h"

int main_compression(int argc, char *argv[]){
    int i, j, k, taille = 0, existe = 0, m = 1, n = 0, o, p;
    int tab[MAX_CHAR] = {0};
    char caractere, nb_n[11] = {0}, code[MAX_PROF] = {0}, ajout_nom[4] = {0}, nb[11] = {0}, *chaine, nom_fichier[MAX_CHAR] = {0}, nom_fichier_temporaire[MAX_CHAR] = {0};
    char **liste_tmp;
    /*noeud *arbre_huffman[MAX_CHAR] = {0}, *alphabet[MAX_CHAR] = {0};*/
    FILE *fichier, *archive;
    if(argc < 4){
        printf("Veuillez entrer les arguments au format -c archive_finale dossiers_ou_fichiers_a_compresser.\n");
        return 0;
    }
    if(access(argv[2], F_OK)==0){
        printf("%s existe déjà.\n", argv[2]);
        existe = 1;
        strcpy(nom_fichier, argv[2]);
    }
    for(i=3;i<argc;i++){
        if(access(argv[i], F_OK)!=0){
            printf("<dossiers_ou_fichiers_à_compresser> n'existe pas.\n");
            return 0;
        }
    }

    j = i - 3; /*nombre de fichiers à compresser*/
    o = j;
    liste_tmp = (char**)malloc(j * sizeof(char*));
    if(liste_tmp == NULL){
        printf("Erreur allocation mémoire.");
        return 0;
    }
    for(i=0;i<j;i++){
        liste_tmp[i] = (char*)malloc(MAX_CHAR * sizeof(char));
        if(liste_tmp[i] == NULL){
            printf("Erreur allocation mémoire.");
            return 0;
        }
        strcpy(liste_tmp[i], "\0");
    }
    
    if(existe == 1){
        while(access(nom_fichier, F_OK)==0 && existe < INT_MAX){
            sprintf(ajout_nom,"%d",existe);
            strcpy(nom_fichier, argv[2]);
            strcat(nom_fichier, ajout_nom);
            existe++;
        }
        if(existe == INT_MAX){
            printf("Impossible de créer l'archive.\n");
            return 0;
        }
        printf("Nouveau nom de l'archive : %s\n", nom_fichier);
        /*creer_fichier(nom_fichier, argv[3], alphabet);*/
    }else{
        strcpy(nom_fichier, argv[2]);
    }
    /*création de l'archive
      Le format de donnée est :
      nb_fichiers
      <liste_fichiers>
      
      maintenant, pour chaque fichier à compresser, on va tous les compresser séparément.
      Puis, on va lire chaque fichier en comptant le nombre de lignes.
      Dans l'archive, le format va être :
      nb_lignes
      <contenu_fichier>
      
      Pour décompresser, il suffira de lire l'en-tête de l'archive pour voir le nombre de fichiers à décompresser.
      Et pour chacun d'entre eux, on lira en utilisant le nombre de lignes indiqué tout le contenu pour chaque fichier, ce contenu sera écrit dans un fichier temporaire à chaque fois et on lancera la décompression à partir de ces fichiers
    */
    fichier = fopen(nom_fichier, "w");
    sprintf(nb, "%d", j);
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
    fclose(fichier);
    for(i=0;i<j;i++){
        noeud *arbre_huffman[MAX_CHAR] = {0}, *alphabet[MAX_CHAR] = {0};
        /*ici on lance la compression pour chaque fichier*/
        /*une fois qu'un fichier est compressé, on l'insère dans l'archive*/
        fichier = fopen(argv[i+3], "r");
        occurence(fichier, tab);
        fclose(fichier);
        taille = 0;
        for(k=0;k<MAX_CHAR;k++){
            arbre_huffman[taille] = creer_feuille(tab, k);
            if(arbre_huffman[taille] == NULL){
                taille--;
            }
            taille++;
        }
        creer_noeud(arbre_huffman, taille);
        code[0] = '0';
        code[1] = '\0';
        creer_code(arbre_huffman[0], alphabet, code, 1);
        strcpy(nom_fichier_temporaire, "tmp1");
        existe = 1;
        while(access(nom_fichier_temporaire, F_OK)==0 && existe < INT_MAX){
            strcpy(nom_fichier_temporaire, "tmp");
            sprintf(ajout_nom, "%d", existe);
            strcat(nom_fichier_temporaire, ajout_nom);
            existe++;
        }
        /*liste des fichiers temporaires*/
        m = 1;
        n = 0;
        while(m == 1){
            if(liste_tmp[n][0] == '\0'){
                strcpy(liste_tmp[n], nom_fichier_temporaire);
                m = 0;
            }
            n++;
        }
        
        if(existe == INT_MAX){
            printf("Impossible de créer l'archive.\n");
            return 0;
        }
        creer_fichier(nom_fichier_temporaire, argv[i+3], alphabet);
        for(k=0;k<MAX_CHAR;k++){
            tab[k] = 0;
            free(arbre_huffman[k]);
            free(alphabet[k]);
        }
    }
    /*maintenant, on a la liste des fichiers temporaires et tous les fichiers ont étés compressés dans ces fichiers temporaires.*/
    archive = fopen(nom_fichier, "a");
    for(i=0;i<o;i++){
        p = 1;
        fichier = fopen(liste_tmp[i], "r");
        do{
            caractere = fgetc(fichier);
            if(feof(fichier)){
                break;
            }else if(caractere == '\n'){
                p++;
            }
        }while(caractere != EOF);
        fclose(fichier);
        sprintf(nb_n, "%d", p);
        fputs(nb_n, archive);
        fputc('\n', archive);
        fichier = fopen(liste_tmp[i], "r");
        do{
            caractere = fgetc(fichier);
            if(feof(fichier)){
                break;
            }
            fputc(caractere, archive);
        }while(caractere != EOF);
        fclose(fichier);
        fputc('\n', archive);
        remove(liste_tmp[i]);
    }
    fclose(archive);
    return 1;
}

int main_decompression(int argc, char *argv[]){
    int i, j, existe = 0, taille, arg = 0, arg2 = 0;
    char *p = NULL, nb[11] = {0}, nom[4097] = {0}, contenu, ajout_nom[4] = {0}, nom_fichier_temporaire[MAX_CHAR] = {0}, nom_fichier[MAX_CHAR] = {0}, fichier2[MAX_CHAR] = {0};
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
    }
    if(argc == 4){
        if(access(argv[3], F_OK)==0){
            existe = 1;
        }
        if((repertoire = opendir(argv[3])) == NULL){
            printf("Création de <dossier_cible>=%s.\n", argv[3]);
            arg = 1;
            arg2 = 1;
        }
    }
    fichier = fopen(argv[2], "r");
    if(fscanf(fichier, "%s", nb) != EOF){
        liste_tmp = (char**)malloc(atoi(nb) * sizeof(char*));
        if(liste_tmp == NULL){
            printf("Erreur allocation mémoire.");
            return 0;
        }
        for(j=0;j<atoi(nb);j++){
            liste_tmp[j] = (char*)malloc(MAX_CHAR * sizeof(char));
            if(liste_tmp[j] == NULL){
                printf("Erreur allocation mémoire.");
                return 0;
            }
            strcpy(liste_tmp[j], "\0");
        }
        liste_origine = (char**)malloc(atoi(nb) * sizeof(char*));
        if(liste_origine == NULL){
            printf("Erreur allocation mémoire.");
            return 0;
        }
        for(j=0;j<atoi(nb);j++){
            liste_origine[j] = (char*)malloc(MAX_CHAR * sizeof(char));
            if(liste_origine[j] == NULL){
                printf("Erreur allocation mémoire.");
                return 0;
            }
            strcpy(liste_origine[j], "\0");
        }
        for(i=0;i<atoi(nb);i++){
            if(fscanf(fichier, "%s", nom) != EOF){
                /*ici on lance la décompression pour chaque fichier*/
                strcpy(liste_origine[i], nom);
            }else{
                return 0;
            }
        }
    }else{
        return 0;
    }
    for(i=0;i<atoi(nb);i++){
        if(fscanf(fichier, "%d", &taille)){
            j = 0;
            fgetc(fichier);
            strcpy(nom_fichier_temporaire, "tmp1");
            existe = 1;
            while(access(nom_fichier_temporaire, F_OK)==0 && existe < INT_MAX){
                strcpy(nom_fichier_temporaire, "tmp");
                sprintf(ajout_nom, "%d", existe);
                strcat(nom_fichier_temporaire, ajout_nom);
                existe++;
            }
            strcpy(liste_tmp[i], nom_fichier_temporaire);
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
        strcpy(fichier2, liste_origine[i]);
        if((p=strrchr(fichier2, '.')) && p != NULL && strstr(p, ".huf")){
            strcpy(p, "\0");
        }

        for(j=0;j<MAX_CHAR;j++){
            nom_fichier[j] = '\0';
        }

        if(argc == 3){
            strcpy(nom_fichier, fichier2);
            while(access(nom_fichier, F_OK)==0 && arg < INT_MAX){
                sprintf(ajout_nom,"%d",arg);
                strcpy(nom_fichier, fichier2);
                strcat(nom_fichier, ajout_nom);
                arg++;
            }
            if(arg == INT_MAX){
                printf("Impossible de créer le fichier.\n");
                return 0;
            }
            creation_fichier(alphabet, nom_fichier, liste_tmp[i]);
        }else{
            if(arg == 1){
                if(mkdir(argv[3], S_IRWXU|S_IRGRP|S_IXGRP) != 0 && access(argv[3], F_OK)!=0){
                    printf("Impossible de créer le répertoire.\n");
                    return 0;
                }
            }
            arg = 1;
            strcat(nom_fichier, argv[3]);
            if(nom_fichier[strlen(nom_fichier)] != '/'){
                strcat(nom_fichier, "/");
            }
            strcat(nom_fichier, fichier2);
            strcpy(fichier2, nom_fichier);
            while(access(nom_fichier, F_OK)==0 && arg < INT_MAX){
                sprintf(ajout_nom,"%d",arg);
                strcpy(nom_fichier, fichier2);
                strcat(nom_fichier, ajout_nom);
                arg++;
            }
            if(arg == INT_MAX){
                printf("Impossible de créer le fichier.\n");
                return 0;
            }
            arg = arg2;
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