#ifndef _FILE_H_
#define _FILE_H_
#include "liste.h"

typedef liste file;

file file_vide();

int est_file_vide(file f);

file enfiler(file f,element e);

element debut_file(file f);

file defiler(file f);

int taille_file(file f);

#endif