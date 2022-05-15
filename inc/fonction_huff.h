#ifndef F_HUFF
#define F_HUFF

/*reçoie le nom du fichier à modifier, vérifie si un autre fichier dans le répertoire courant porte déjà le même nom, et si c'est le cas change le nom du fichier jusqu'à trouver un nom inutilisé*/
/*il est nécessaire de se déplacer dans le répertoire où le fichier doit être créé à l'aide de chdir pour utiliser cette fonction*/
char * change_nom_fichier(char ancien_nom[MAX_CHAR]);

/*crée une liste de taille taille*/
char ** creer_liste(int taille);

/*retourne la liste des fichiers dans le réperoire donné en paramètre*/
char ** liste_repertoire(char chemin[MAX_CHEMIN]);

/*prend en paramètre le répertoire dans lequel faire la recherche et la liste des fichiers de ce répertoire avant compression
Retourne la liste des nouveaux fichiers créés depuis la compression
Cette liste contient la liste des fichiers temporaires qui sont ceux à compresser en une seule archive*/
char ** liste_fichier_tmp(char chemin[MAX_CHEMIN], char **liste);
#endif