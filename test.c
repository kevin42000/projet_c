#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <dirent.h>
void usage(char*v[]){
    printf("Usage : %s (-p <chemin de destination des fichiers>) <nombre de fichier a creer 1-999> (<taille des fichier 0-INT_MAX>) (<nombre de caractere a utiliser 0-255 (ascii = 10 + 32 a 128)>)\n",v[0]);
    exit(-1);
}
char* strsub(char* in,int start,int len){
    int length = strlen(in);
    char *out = NULL;
    if(start >= length){
        printf("strsub : start out of range");
    }
    if(start+len >= length){
        printf("strsub : start+len out of range");
    }
    /* safety first comme diraient les anglais */
    if(len>0){
        printf("%ld\n",(len+1) * sizeof(char));
        out = (char*)calloc((len+1),sizeof(char));
        if(out == NULL){
            printf("Erreur alloc\n");
            exit(-1);
        }
        puts("Ce");
        strncat(out,in+start,len);/* on concat seulement les len char de in depuis start dans out */
        free(in);
        return out;
    }
    return in;
}
int gen_file(char* name,int size,int nb_char){
    FILE *f = fopen(name,"w");
    char c;
    char progress[23];
    int i;
    int max_size = size;
    double al = ((double)(max_size-size)/(double)max_size)*100.0;
    if(f == NULL){
        printf("Impossible de creer des fichiers ...\n");
        exit(-1);
    }
    printf("Starting creating %s with %d chars and length of %d\n",name,nb_char,size);
    progress[0] = '[';
    progress[21] = ']';
    progress[22] = '\0';
    while(size > 0){
        if(rand()%100 == 0 && nb_char > 2){
            c = '\n';
        }else{
            c = rand() % nb_char + 32;
        }
        fputc(c,f);
        size--;
        al = ((double)(max_size-size)/(double)max_size)*100.0;
        for(i=1;i<=20;i++){
            progress[i] = (5.0*i-1) < al ? '*' : '-';
        }
        printf("\r %.1f%% %s",al,progress);
        fflush(stdout);
    }
    fclose(f);
}
int main(int c,char *v[]){
    int x = 0,i = -1,j,y = 0,z = 0;
    char *filename;
    char *path;
    DIR* dir;
    if(c < 2){
        usage(v);
    }
    srand(time(NULL));
    for(j=1;j<c;j++){
        if(strlen(v[j]) == 2){
            if(v[j][0] == '-' && v[j][1] == 'p'){
                i = j+1;
                break;
            }
        }
    }
    if(i > -1 && i < c){
        dir = opendir(v[i]);
        if(dir == NULL){
            printf("Le repertoire n'existe pas ou est innaccessible !\n");
            exit(-1);
        }
        closedir(dir);/*Il existe forcement d'autre moyen de regarder si le dossier existe mais bon celle-ci fonctionne :)*/
        path = (char*)malloc(sizeof(char) * strlen(v[i]) + 12);
        filename = (char*)malloc(sizeof(char) * strlen(v[i]) + 12);
        if(path == NULL || filename == NULL){
            printf("Erreur allocation\n");
            exit(-1);
        }
        strcpy(path,v[i]);
        x = atoi(v[3]);
    if(c > 5){
        y = atoi(v[4]);
    }
    if(c > 6){
        z = atoi(v[5]);
    }
    }else{
    x = atoi(v[1]);
    if(c > 3){
        y = atoi(v[2]);
    }
    if(c > 4){
        z = atoi(v[3]);
    }
    }
    if(x <= 0 || x > 999){
        usage(v);
    }
    for(i=0;i<x;i++){
        sprintf(filename,"%stest%d.txt",path,i);
        if(y <= 0){
            y = rand()%10000000;
        }
        if(z <= 0){
            z = rand()%(128-32);
        }
        gen_file(filename,y,z);
        printf("\n");
    }
}
