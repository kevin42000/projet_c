#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
int main_compression(int argv, char *argc[]){
    int i, j;
    char nb[11] = {0}, *chaine;
    FILE *fichier;
    if(argv < 4){
        printf("Veuillez entrer les arguments au format -c archive_finale dossiers_ou_fichiers_a_compresser.\n");
        return 0;
    }
    if(access(argc[2], F_OK)==0){
        printf("<archive_finale> existe déjà.\n");
        return 0;
    }
    for(i=3;i<argv;i++){
        if(access(argc[i], F_OK)!=0){
            printf("<dossiers_ou_fichiers_à_compresser> n'existe pas.\n");
            return 0;
        }
    }

    j = i - 2; /*nombre de fichiers à compresser*/
    fichier = fopen(argc[2], "w");
    sprintf(nb, "%d", j);
    fputs(nb, fichier);
    fputc('\n', fichier);
    for(i=3;i<argv;i++){
        if((chaine = strrchr(argc[i], '/')) != NULL){
            fputs((chaine + 1), fichier);
        }else{
            fputs(argc[i], fichier);
        }
        fputc('\n', fichier);
    }
    fclose(fichier);

    for(i=0;i<j;i++){
        /*ici on lance la compression pour chaque fichier*/
        /*une fois qu'un fichier est compressé, on l'insère dans l'archive*/
    }
    return 1;
}

int main_decompression(int argv, char *argc[]){
    int i;
    char nb[11] = {0}, nom[4097] = {0};
    FILE *fichier;
    if(argv !=3 && argv !=4){
        printf("Veuillez entrer les arguments au format -d archive_a_décompresser dossier_cible.\n");
        return 0;
    }
    if(access(argc[2], F_OK)!=0){
        printf("<archive_à_décompresser> n'existe pas.\n");
        return 0;
    }
    if(argv == 4 && access(argc[3], F_OK)==0){
        printf("<dossier_cible> existe déjà.\n");
        return 0;
    }
    fichier = fopen(argc[2], "r");
    if(fscanf(fichier, "%[^\n]", nb) != EOF){
        for(i=0;i<atoi(nb);i++){
            if(fscanf(fichier, "%[^\n]", nom) != EOF){
                /*ici on lance la décompression pour chaque fichier*/
            }
        }
    }
    fclose(fichier);
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

int main(int argv, char *argc[]){
    int option;
    if((option = getopt(argv, argc, "cdh")) != -1){
        switch(option){
        case('c'):
            main_compression(argv, argc);
            break;
        case('d'):
            main_decompression(argv, argc);
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