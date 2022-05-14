#include <stdio.h>
#include <stdlib.h>
#include <time.h>
int gen_file(char* name,int size,int nb_char){
    FILE *f = fopen(name,"w");
    char c;
    char progress[23];
    int i;
    int max_size = size;
    double al = ((double)(max_size-size)/(double)max_size)*100.0;
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
    int x,i,j;
    char filename[12] = "test000.txt";
    srand(time(NULL));
    if(c < 2){
        printf("Usage : %s <nombre de fichier à créer>\n",v[0]);
        exit(-1);
    }
    x = atoi(v[1]);
    for(i=0;i<x;i++){
        sprintf(filename,"test%d.txt",i);
        j = rand()%(RAND_MAX/100);
        printf("Creating a file of %d chars\n",j);
        gen_file(filename,j,2);
        printf("\n");
    }
}