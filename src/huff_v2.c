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

int main_compression_fichier(int argc, char *argv[]){
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
        free(arbre_huffman[i]);
        free(alphabet[i]);
    }
    return 1;
}

int main_compression(int argc, char *argv[]){
    int i = 0, j, p;
    char caractere, chemin[MAX_PROF] = {0}, *argv2[4], nb[11] = {0}, *chaine, nom_fichier[MAX_CHAR] = {0};
    char **liste_avant, **liste_tmp, **liste_source, **liste_tmp2;
    /*noeud *arbre_huffman[MAX_CHAR] = {0}, *alphabet[MAX_CHAR] = {0};*/
    FILE *fichier, *tmp;
    DIR *repertoire;
    struct dirent *liste;
    if(argc != 4){
        printf("Veuillez entrer les arguments au format -c archive_finale dossiers_à_compresser.\n");
        return 0;
    }
    if(access(argv[2], F_OK)==0){
        printf("%s existe déjà.\n", argv[2]);
    }
    repertoire = opendir(argv[3]);
    if(access(argv[3], F_OK)!=0){
        printf("<fichier_ou_dossier_à_compresser> n'existe pas.\n");
        return 0;
    }else if(repertoire == NULL){
        return main_compression_fichier(argc, argv);
    }
    
    while((liste = readdir(repertoire))){
        if(liste->d_type == 4 && strcmp(liste->d_name, ".") != 0 && strcmp(liste->d_name, "..") != 0){
            printf("Il y a des répertoires dans votre répertoire à compresser.\n");
            return 0;
        }else if(liste->d_type == 8){
            i++;
        }
    }
    closedir(repertoire);
    
    liste_source = creer_liste(i);
    liste_tmp = creer_liste(i);
    
    i = 0;
    /*liste_source contient la liste des fichiers à compresser*/
    strcpy(chemin, "./");
    strcat(chemin, argv[3]);
    if(chemin[strlen(chemin)] != '/'){
        strcat(chemin, "/");
    }
    repertoire = opendir(chemin);
    while((liste = readdir(repertoire))){
        if(liste->d_type == 8){
            strcpy(liste_source[i], liste->d_name);
            i++;
        }
    }
    closedir(repertoire);
    for(j=0;j<i;j++){
        liste_avant = liste_repertoire(chemin);
        argv2[0] = NULL;
        argv2[1] = NULL;
        argv2[2] = (char*)malloc(4 * sizeof(char));
        argv2[3] = (char*)malloc((strlen(liste_source[j])+1) * sizeof(char));
        strcpy(nom_fichier, "tmp");
        sprintf(nb, "%d", j);
        strcat(nom_fichier, nb);
        strcpy(argv2[2], nom_fichier);
        strcpy(argv2[3], liste_source[j]);
        if(chdir(chemin) != 0){
            return 0;
        }
        main_compression_fichier(4, argv2);
        if(chdir("../") != 0){
            return 0;
        }
        liste_tmp2 = liste_fichier_tmp(chemin, liste_avant);
        strcpy(liste_tmp[j], liste_tmp2[0]);
    }
    strcpy(nom_fichier, argv[2]);
    strcpy(nom_fichier, change_nom_fichier(nom_fichier));
    printf("Nom de l'archive: %s\n", nom_fichier);
    fichier = fopen(nom_fichier, "w");
    sprintf(nb, "%d", i+1);
    fputs(nb, fichier);
    fputc('\n', fichier);
    fputs(argv[3], fichier);
    fputc('\n', fichier);
    for(j=0;j<i;j++){
        if((chaine = strrchr(liste_source[j], '/')) != NULL){
            fputs((chaine + 1), fichier);
        }else{
            fputs(liste_source[j], fichier);
        }
        fputc('\n', fichier);
    }
    if(chdir(chemin) != 0){
        return 0;
    }
    for(j=0;j<i;j++){
        p = 1;
        tmp = fopen(liste_tmp[i], "r");
        do{
            caractere = fgetc(tmp);
            if(feof(tmp)){
                break;
            }else if(caractere == '\n'){
                p++;
            }
        }while(caractere != EOF);
        fclose(tmp);
        sprintf(nb, "%d", p);
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
    /*if(existe == 1){
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
        printf("Nouveau nom de l'archive : %s\n", nom_fichier);*/
        /*creer_fichier(nom_fichier, argv[3], alphabet);*/
    /*}else{
        strcpy(nom_fichier, argv[2]);
    }*/
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
    /*fichier = fopen(nom_fichier, "w");
    sprintf(nb, "%d", i+1);
    fputs(nb, fichier);
    fputc('\n', fichier);
    fputs(argv[3], fichier);
    fputc('\n', fichier);
    for(j=0;j<i;j++){
        if((chaine = strrchr(liste_source[j], '/')) != NULL){
            fputs((chaine + 1), fichier);
        }else{
            fputs(liste_source[j], fichier);
        }
        fputc('\n', fichier);
    }
    fclose(fichier);
    
    for(i=0;i<j;i++){
    noeud *arbre_huffman[MAX_CHAR] = {0}, *alphabet[MAX_CHAR] = {0};*/
        /*ici on lance la compression pour chaque fichier*/
        /*une fois qu'un fichier est compressé, on l'insère dans l'archive*/
        /*strcpy(nom_fichier_temporaire, argv[3]);
        strcat(nom_fichier_temporaire, "/");
        strcat(nom_fichier_temporaire, liste_source[i]);
        strcpy(liste_source[i], nom_fichier_temporaire);
        fichier = fopen(liste_source[i], "r");
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
            }*/
        /*liste des fichiers temporaires*/
        /*m = 1;
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
        creer_fichier(nom_fichier_temporaire, liste_source[i], alphabet);
        for(k=0;k<MAX_CHAR;k++){
            tab[k] = 0;
            free(arbre_huffman[k]);
            free(alphabet[k]);
        }
        }*/
    /*maintenant, on a la liste des fichiers temporaires et tous les fichiers ont étés compressés dans ces fichiers temporaires.*/
    /*archive = fopen(nom_fichier, "a");
      for(i=0;i<o;i++){*/
/*        p = 1;
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
    return 1;*/
}

int main_decompression_fichier(int argc, char *argv[]){
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
    int i, j, existe = 0, taille, arg = 0, arg2 = 0;
    char *p = NULL, nb[11] = {0}, nom[4097] = {0}, contenu, ajout_nom[4] = {0}, nom_fichier_temporaire[MAX_CHAR] = {0}, nom_fichier[MAX_CHAR] = {0}, fichier2[MAX_CHAR] = {0}, rep[MAX_CHAR] = {0};
    char **liste_tmp, **liste_origine;
    FILE *fichier, *tmp;
    DIR *repertoire;
    struct dirent *liste;
    if(argc !=3 && argc !=4){
        printf("Veuillez entrer les arguments au format -d archive_a_décompresser dossier_cible.\n");
        return 0;
    }
    if(access(argv[2], F_OK)!=0){
        printf("<archive_à_décompresser> n'existe pas.\n");
        return 0;
    }
    fichier = fopen(argv[2], "r");
    if(fscanf(fichier, "%s", nb) != EOF){
        if(!isdigit(nb[0])){
            fclose(fichier);
            return main_decompression_fichier(argc, argv);
        }
        fclose(fichier);
    }else{
        fclose(fichier);
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
        closedir(repertoire);
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
    for(i=1;i<atoi(nb);i++){
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
    
    for(i=1;i<atoi(nb);i++){
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
            strcpy(nom_fichier, liste_origine[0]);
            strcat(nom_fichier, "/");
            if(mkdir(liste_origine[0], S_IRWXU|S_IRGRP|S_IXGRP) != 0 && access(liste_origine[0], F_OK)!=0){
                printf("Impossible de créer le répertoire.\n");
                return 0;
            }
            repertoire = opendir("./");
            while((liste = readdir(repertoire))){
                if(strcmp(liste->d_name, liste_origine[0])==0 && liste->d_type == 8){
                    printf("Impossible de créer le répertoire car il y a un fichier portant le même nom.\n");
                    return 0;
                }
            }
            closedir(repertoire);
            
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
            creation_fichier(alphabet, nom_fichier, liste_tmp[i]);
        }else{
            if(arg == 1){
                if(mkdir(argv[3], S_IRWXU|S_IRGRP|S_IXGRP) != 0 && access(argv[3], F_OK)!=0){
                    printf("Impossible de créer le répertoire.\n");
                    return 0;
                }
                repertoire = opendir("./");
                while((liste = readdir(repertoire))){
                    if(strcmp(liste->d_name, argv[3])==0 && liste->d_type == 8){
                        printf("Impossible de créer le répertoire car il y a un fichier portant le même nom.\n");
                        return 0;
                    }
                }
                closedir(repertoire);
            }
            strcpy(nom_fichier, argv[3]);
            strcat(nom_fichier, "/");
            strcat(nom_fichier, liste_origine[0]);
            strcat(nom_fichier, "/");
            if(mkdir(nom_fichier, S_IRWXU|S_IRGRP|S_IXGRP) != 0 && access(nom_fichier, F_OK)!=0){
                printf("Impossible de créer le répertoire.\n");
                return 0;
            }
            strcpy(rep, "./");
            strcat(rep, argv[3]);
            repertoire = opendir(rep);
            while((liste = readdir(repertoire))){
                if(strcmp(liste->d_name, liste_origine[0])==0 && liste->d_type == 8){
                    printf("Impossible de créer le répertoire car il y a un fichier portant le même nom.\n");
                    return 0;
                }
            }
            closedir(repertoire);
            
            arg = 1;
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