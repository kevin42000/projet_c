#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <dirent.h>
void usage(char *v[])
{
    printf("Usage : %s (-p <chemin de destination des fichiers>) -n <nombre de fichier a creer 1-999> (-t <taille des fichier 0-INT_MAX>) (-c <nombre de caractere a utiliser 0-255 (ascii = 10 + 32 a 128)>)\n", v[0]);
    exit(-1);
}

int get_opt_val(int argc, char *argv[], char query)
{
    int i;
    for (i = 0; i < argc; i++)
    {
        if (strlen(argv[i]) == 2)
        {
            if (argv[i][0] == '-' && argv[i][1] == query)
            {
                if (i < argc - 1)
                {
                    return i + 1;
                }
            }
        }
    }
    return -1;
}

char *strsub(char *in, int start, int len)
{
    int length = strlen(in);
    char *out = NULL;
    if (start >= length)
    {
        printf("strsub : start out of range");
    }
    if (start + len >= length)
    {
        printf("strsub : start+len out of range");
    }
    /* safety first comme diraient les anglais */
    if (len > 0)
    {
        printf("%ld\n", (len + 1) * sizeof(char));
        out = (char *)calloc((len + 1), sizeof(char));
        if (out == NULL)
        {
            printf("Erreur alloc\n");
            exit(-1);
        }
        puts("Ce");
        strncat(out, in + start, len); /* on concat seulement les len char de in depuis start dans out */
        free(in);
        return out;
    }
    return in;
}

int gen_file(char *name, int size, int nb_char)
{
    FILE *f = fopen(name, "w");
    char c;
    char progress[23];
    int i;
    int max_size = size;
    double al = ((double)(max_size - size) / (double)max_size) * 100.0;
    if (f == NULL)
    {
        printf("Impossible de creer des fichiers ...\n");
        exit(-1);
    }
    printf("Starting creating %s with %d chars and length of %d\n", name, nb_char, size);
    progress[0] = '[';
    progress[21] = ']';
    progress[22] = '\0';
    while (size > 0)
    {
        if (rand() % 100 == 0 && nb_char > 2)
        {
            c = '\n';
        }
        else
        {
            c = rand() % nb_char + 32;
        }
        fputc(c, f);
        size--;
        al = ((double)(max_size - size) / (double)max_size) * 100.0;
        for (i = 1; i <= 20; i++)
        {
            progress[i] = (5.0 * i - 1) < al ? '*' : '-';
        }
        printf("\r %.1f%% %s", al, progress);
        fflush(stdout);
    }
    fclose(f);
}

int main(int c, char *v[])
{
    int x = 0, i = -1, j, y = 0, z = 0;
    char *filename;
    char *path;
    DIR *dir;
    if (c < 2)usage(v);
    srand(time(NULL));
    /* nombre de fichiers */
    if ((i = get_opt_val(c, v, 'n')) == -1)
    {
        usage(v);
    }
    x = atoi(v[i]);
    if (x <= 0 || x > 999)
        usage(v);
    /* dossier de création */
    if ((i = get_opt_val(c, v, 'p')) > -1)
    {
        if ((dir = opendir(v[i])) == NULL)
        {
            printf("Le repertoire n'existe pas ou est innaccessible !\n");
            exit(-1);
        }
        closedir(dir); /* on en a plus besoin de toute manière */
        path = (char *)malloc(sizeof(char) * strlen(v[i]) + 12);
        filename = (char *)malloc(sizeof(char) * strlen(v[i]) + 12);
        if (path == NULL || filename == NULL)
        {
            printf("Erreur allocation\n");
            exit(-1);
        }
        strcpy(path, v[i]);
    }
    else
    {
        path = (char *)malloc(sizeof(char) * 15);
        filename = (char *)malloc(sizeof(char) * 15);
        strcpy(path, "./");
    }

    /* taille du fichier */
    if ((i = get_opt_val(c, v, 't')) != -1)
    {
        y = atoi(v[i]);
        if (y <= 0)
        {
            usage(v);
        }
    }
    else
    {
        y = -1;
    }

    /* nombre de caractère à utiliser */
    if ((i = get_opt_val(c, v, 'c')) != -1)
    {
        z = atoi(v[i]);
        if (z <= 0 || z > 255)
        {
            usage(v);
        }
    }
    else
    {
        z = -1;
    }

    for (i = 0; i < x; i++)
    {
        srand(time(NULL));/* certains fichiers peuvent prendre du temps à écrire */
        sprintf(filename, "%stest%d.txt", path, i);
        j=0;
        if (y <= 0)
        {
            j += 1;
            y = rand() % 10000000; /* entre 0o et 10Mo */
        }
        if (z <= 0)
        {
            j += 2;
            z = rand() % (128 - 32);
        }
        gen_file(filename, y, z);
        if(j == 3 ){
            y = 0;
            z = 0;
        }else if(j == 2){
            z = 0;
        }else if(j == 1){
            y = 0;
        }
        printf("\n");
    }
}
