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
    char code[MAX_PROF] = {0}, ajout_nom[4] = {0}, nom_fichier[MAX_CHAR] = {0};
    noeud *arbre_huffman[MAX_CHAR] = {0}, *alphabet[MAX_CHAR] = {0};
    FILE *fichier;
    if(argc !=4){
        printf("Veuillez entrer les arguments au format -c archive_finale fichier_à_compresser.\n");
        return 0;
    }
    if(access(argv[2], F_OK)==0){
        printf("<archive_finale> existe déjà.\n");
        existe = 1;
        strcpy(nom_fichier, argv[2]);
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
    if(existe == 0){
        creer_fichier(argv[2], argv[3], alphabet);
    }else{
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
        creer_fichier(nom_fichier, argv[3], alphabet);
    }
    for(i=0;i<MAX_CHAR;i++){
        free(alphabet[i]);
    }
    return 1;
}

int main_compression_fichier_multiple(int argc, char *argv[]){
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

int main_compression_dossier(int argc, char *argv[]){
    int i, j, k, taille = 0, existe = 0, m = 1, n = 0, o, p;
    int tab[MAX_CHAR] = {0};
    char caractere, nb_n[11] = {0}, code[MAX_PROF] = {0}, ajout_nom[4] = {0}, nb[11] = {0}, *chaine, nom_fichier[MAX_CHAR] = {0}, nom_fichier_temporaire[MAX_CHAR] = {0};
    char **liste_tmp, **liste_source;
    /*noeud *arbre_huffman[MAX_CHAR] = {0}, *alphabet[MAX_CHAR] = {0};*/
    FILE *fichier, *archive;
    DIR *repertoire;
    struct dirent *liste;
    if(argc < 4){
        printf("Veuillez entrer les arguments au format -c archive_finale dossiers_à_compresser.\n");
        return 0;
    }
    if(access(argv[2], F_OK)==0){
        printf("%s existe déjà.\n", argv[2]);
        existe = 1;
        strcpy(nom_fichier, argv[2]);
    }
    repertoire = opendir(argv[3]);
    if(access(argv[3], F_OK)!=0 && repertoire == NULL){
        printf("<fichier_ou_dossier_à_compresser> n'existe pas.\n");
        return 0;
    }
    
    if(repertoire == NULL){
        return main_compression_fichier(argc, argv);
    }
    
    i = 0;
    while((liste = readdir(repertoire))){
        if(liste->d_type == 4 && strcmp(liste->d_name, ".") != 0 && strcmp(liste->d_name, "..") != 0){
            printf("Il y a des répertoires dans votre répertoire à compresser.\n");
            return 0;
        }else if(liste->d_type == 8){
            i++;
        }
    }
    closedir(repertoire);
    
    j = i; /*nombre de fichiers à compresser*/
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
    
    liste_source = (char**)malloc(j * sizeof(char*));
    if(liste_source == NULL){
        printf("Erreur allocation mémoire.");
        return 0;
    }
    for(i=0;i<j;i++){
        liste_source[i] = (char*)malloc(MAX_CHAR * sizeof(char));
        if(liste_source[i] == NULL){
            printf("Erreur allocation mémoire.");
            return 0;
        }
        strcpy(liste_source[i], "\0");
    }
    
    if((repertoire = opendir(argv[3])) == NULL){
        printf("<répertoire_cible>=%s n'existe pas.\n", argv[3]);
        return 0;
    }
    i = 0;
    /*liste_source contient la liste des fichiers à compresser*/
    while((liste = readdir(repertoire))){
        if(liste->d_type == 8){
            strcpy(liste_source[i], liste->d_name);
            i++;
        }
    }
    closedir(repertoire);
    
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
    sprintf(nb, "%d", j+1);
    fputs(nb, fichier);
    fputc('\n', fichier);
    fputs(argv[3], fichier);
    fputc('\n', fichier);
    for(i=0;i<j;i++){
        if((chaine = strrchr(liste_source[i], '/')) != NULL){
            fputs((chaine + 1), fichier);
        }else{
            fputs(liste_source[i], fichier);
        }
        fputc('\n', fichier);
    }
    fclose(fichier);
    
    for(i=0;i<j;i++){
        noeud *arbre_huffman[MAX_CHAR] = {0}, *alphabet[MAX_CHAR] = {0};
        /*ici on lance la compression pour chaque fichier*/
        /*une fois qu'un fichier est compressé, on l'insère dans l'archive*/
        strcpy(nom_fichier_temporaire, argv[3]);
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
        creer_fichier(nom_fichier_temporaire, liste_source[i], alphabet);
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

int main_compression(int argc, char *argv[]){
    int i, j, k, l, existe = 0, retour = 1, p;
    char caractere, *chaine, **liste_tmp, **liste_source, *argv2[4], nom_fichier[MAX_CHAR] = {0}, ajout_nom[11] = {0}, nb[11] = {0}, nb_n[11] = {0};
    FILE *fichier, *archive;
    DIR *repertoire;
    struct dirent *liste;
    if(argc < 4){
        printf("Veuillez entrer les arguments au format -c archive_finale dossiers_à_compresser.\n");
        return 0;
    }
    if(access(argv[2], F_OK)==0){
        printf("%s existe déjà.\n", argv[2]);
        existe = 1;
        strcpy(nom_fichier, argv[2]);
    }
    repertoire = opendir(argv[3]);
    if(access(argv[3], F_OK)!=0 && repertoire == NULL){
        printf("<fichier_ou_dossier_à_compresser> n'existe pas.\n");
        return 0;
    }
    
    if(repertoire == NULL){
        return main_compression_fichier(argc, argv);
    }
    i = 0;
    j = 0;
    while((liste = readdir(repertoire))){
        if(liste->d_type == 4 && strcmp(liste->d_name, ".") != 0 && strcmp(liste->d_name, "..") != 0){
                
            i++;
            j = 1;
        }else if(liste->d_type == 8){
            i++;
        }
    }
    closedir(repertoire);
    
    if(j == 0){
        return main_compression_dossier(argc, argv);
    }
    
    liste_tmp = (char**)malloc(i * sizeof(char*));
    if(liste_tmp == NULL){
        printf("Erreur allocation mémoire.");
        return 0;
    }
    for(k=0;k<i;k++){
        liste_tmp[k] = (char*)malloc(MAX_CHAR * sizeof(char));
        if(liste_tmp[k] == NULL){
            printf("Erreur allocation mémoire.");
            return 0;
        }
        strcpy(liste_tmp[k], "\0");
    }
    
    liste_source = (char**)malloc(i * sizeof(char*));
    if(liste_source == NULL){
        printf("Erreur allocation mémoire.");
        return 0;
    }
    for(k=0;k<i;k++){
        liste_source[k] = (char*)malloc(MAX_CHAR * sizeof(char));
        if(liste_source[k] == NULL){
            printf("Erreur allocation mémoire.");
            return 0;
        }
        strcpy(liste_source[k], "\0");
    }
    i = 0;
    repertoire = opendir(argv[3]);
    while((liste = readdir(repertoire))){
        if((liste->d_type == 4 && strcmp(liste->d_name, ".") != 0 && strcmp(liste->d_name, "..") != 0) || (liste->d_type == 8)){
            strcpy(liste_tmp[i], liste->d_name);
            i++;
        }
    }
    closedir(repertoire);
    if(chdir(argv[3]) != 0){
        return 0;
    }
    argv2[0] = NULL;
    argv2[1] = NULL;
    for(j=0;j<i;j++){
        argv2[2] = (char*)malloc((strlen(liste_tmp[j])+1) * sizeof(char));
        argv2[3] = (char*)malloc((strlen(liste_tmp[j])+1) * sizeof(char));
        if(argv2[2] == NULL || argv2[3] == NULL){
            printf("Erreur allocation mémoire.\n");
            return 0;
        }
        strcpy(argv2[2], liste_tmp[j]);
        strcpy(argv2[3], liste_tmp[j]);
        retour = main_compression_dossier(4, argv2);
        if(retour == 0){
            return 0;
        }
    }
    l = 0;
    if(chdir("../") != 0){
        return 0;
    }
    repertoire = opendir(argv[3]);
    while((liste = readdir(repertoire))){
        if((liste->d_type == 4 && strcmp(liste->d_name, ".") != 0 && strcmp(liste->d_name, "..") != 0) || (liste->d_type == 8)){
            k = 0;
            for(j=0;j<i;j++){
                if(strcmp(liste->d_name, liste_tmp[j]) == 0){
                    k = 1;
                    break;
                }
            }
            if(k == 0){
                strcpy(liste_source[l], liste->d_name);
                l++;
            }
        }
    }
    closedir(repertoire);
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
    fichier = fopen(nom_fichier, "w");
    sprintf(nb, "%d", l+1);
    fputs(nb, fichier);
    fputc('\n', fichier);
    fputs(argv[3], fichier);
    fputc('\n', fichier);
    for(i=0;i<j;i++){
        if((chaine = strrchr(liste_source[i], '/')) != NULL){
            fputs((chaine + 1), fichier);
        }else{
            fputs(liste_source[i], fichier);
        }
        fputc('\n', fichier);
    }
    fclose(fichier);
    archive = fopen(nom_fichier, "a");
    if(chdir(argv[3]) != 0){
        return 0;
    }
    for(i=0;i<l;i++){
        p = 1;
        fichier = fopen(liste_source[i], "r");
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
        fichier = fopen(liste_source[i], "r");
        do{
            caractere = fgetc(fichier);
            if(feof(fichier)){
                break;
            }
            fputc(caractere, archive);
        }while(caractere != EOF);
        fclose(fichier);
        fputc('\n', archive);
        remove(liste_source[i]);
    }
    if(chdir("../") != 0){
        return 0;
    }
    return 1;
}

int main_decompression_fichier(int argc, char *argv[]){
    int arg = 0;
    char *p = NULL, fichier[MAX_CHAR] = {0}, ajout_nom[4] = {0}, nom_fichier[MAX_CHAR] = {0};
    noeud *alphabet[MAX_CHAR] = {0};
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
    if(argc == 4){
        if((repertoire = opendir(argv[3])) == NULL){
            printf("Création de <dossier_cible>=%s.\n", argv[3]);
            arg = 1;
        }
        closedir(repertoire);
    }
    decompression(alphabet, argv[2]);
    strcpy(fichier, argv[2]);
    if((p=strrchr(fichier, '.')) && p != NULL && strstr(p, ".huf")){
        strcpy(p, "\0");
    }
    if(argc == 3){
        strcpy(nom_fichier, fichier);
        while(access(nom_fichier, F_OK)==0 && arg < INT_MAX){
            sprintf(ajout_nom,"%d",arg);
            strcpy(nom_fichier, fichier);
            strcat(nom_fichier, ajout_nom);
            arg++;
        }
        if(arg == INT_MAX){
            printf("Impossible de créer le fichier.\n");
            return 0;
        }
        creation_fichier(alphabet, nom_fichier, argv[2]);
    }else{
        if(arg == 1){
            if(mkdir(argv[3], S_IRWXU|S_IRGRP|S_IXGRP) != 0 && access(argv[3], F_OK)!=0){
                printf("Impossible de créer le répertoire.\n");
                return 0;
            }
            repertoire = opendir("./");
            while((liste = readdir(repertoire))){
                if(strcmp(liste->d_name, argv[3])==0){
                    printf("Impossible de créer le répertoire car il y a un fichier portant le même nom.\n");
                    return 0;
                }
            }
            closedir(repertoire);
        }
        arg = 1;
        strcat(nom_fichier, argv[3]);
        if(nom_fichier[strlen(nom_fichier)] != '/'){
            strcat(nom_fichier, "/");
        }
        strcat(nom_fichier, fichier);
        strcpy(fichier, nom_fichier);
        while(access(nom_fichier, F_OK)==0 && arg < INT_MAX){
            sprintf(ajout_nom,"%d",arg);
            strcpy(nom_fichier, fichier);
            strcat(nom_fichier, ajout_nom);
            arg++;
        }
        if(arg == INT_MAX){
            printf("Impossible de créer le fichier.\n");
            return 0;
        }
        creation_fichier(alphabet, nom_fichier, argv[2]);
    }
    return 1;
}

int main_decompression_dossier(int argc, char *argv[]){
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

int main_decompression(int argc, char *argv[]){
    int i, j, taille, retour = 1;
    char nb[11] = {0}, nom[4097] = {0}, contenu, nom_fichier_temporaire[MAX_CHAR] = {0};
    char **liste_tmp, **liste_origine, **liste_comp;
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
        }
        if((repertoire = opendir(argv[3])) == NULL){
            printf("Création de <dossier_cible>=%s.\n", argv[3]);
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

    if(argc == 3){
        if(mkdir(liste_origine[0], S_IRWXU|S_IRGRP|S_IXGRP) != 0 && access(liste_origine[0], F_OK)!=0){
                printf("Impossible de créer le répertoire.\n");
                return 0;
        }
        if(chdir(liste_origine[0]) != 0){
            return 0;
        }
    }else{
        if(mkdir(argv[3], S_IRWXU|S_IRGRP|S_IXGRP) != 0 && access(argv[3], F_OK)!=0){
                printf("Impossible de créer le répertoire.\n");
                return 0;
        }
        if(chdir(argv[3]) != 0){
            return 0;
        }
        if(mkdir(liste_origine[0], S_IRWXU|S_IRGRP|S_IXGRP) != 0 && access(liste_origine[0], F_OK)!=0){
                printf("Impossible de créer le répertoire.\n");
                return 0;
        }
        if(chdir(liste_origine[0]) != 0){
            return 0;
        }
    }
    for(i=1;i<atoi(nb);i++){
        if(fscanf(fichier, "%d", &taille)){
            j = 0;
            fgetc(fichier);
            strcpy(nom_fichier_temporaire, liste_origine[i]);
            /*existe = 1;
            while(access(nom_fichier_temporaire, F_OK)==0 && existe < INT_MAX){
                strcpy(nom_fichier_temporaire, liste_origine[i]);
                sprintf(ajout_nom, "%d", existe);
                strcat(nom_fichier_temporaire, ajout_nom);
                existe++;
                }*/
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
    liste_comp = (char**)malloc(atoi(nb) * sizeof(char*));
    liste_comp[0] = NULL;
    liste_comp[1] = NULL;
    for(i=0;i<atoi(nb)-1;i++){
        liste_comp[2] = (char*)malloc((strlen(liste_tmp[i+1])+1) * sizeof(char));
        if(liste_comp[2] == NULL){
            printf("Erreur allocation mémoire.");
            return 0;
        }
        strcpy(liste_comp[2], liste_tmp[i+1]);
        retour = main_decompression_dossier(3, liste_comp);
        if(retour == 0){
            return retour;
        }
        remove(liste_comp[2]);
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