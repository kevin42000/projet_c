#ifndef DEF_ARBRE
#define DEF_ARBRE
#define MAX_CHAR 257
#define MAX_PROF 33

typedef struct noeud{
  char caractere;
  int occurence;
  char codage[MAX_PROF];
  int nb_bits;
  struct noeud *gauche;
  struct noeud *droit;
}noeud;

noeud * creer_feuille(int *tab, int index);

void parcours_tableau(noeud *arbre_huffman[MAX_CHAR], int taille, int *x, int *y);

void creer_noeud(noeud *tab[], int taille);

int est_feuille(noeud *noeud_courant);

void creer_code(noeud *noeud_courant, noeud *retour[MAX_CHAR], char code[MAX_PROF], int profondeur);

#endif
