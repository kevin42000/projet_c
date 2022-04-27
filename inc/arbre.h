#ifndef DEF_ARBRE
#define DEF_ARBRE

typedef struct noeud{
  char caractere;
  int occurence;
  char codage[32];
  int nb_bits;
  struct noeud *gauche;
  struct noeud *droit;
}noeud;

noeud * creer_feuille(int *tab, int index);

void parcours_tableau(noeud *arbre_huffman[256], int taille, int *x, int *y);

void creer_noeud(noeud *tab[], int taille);

int est_feuille(noeud *noeud_courant);

void creer_code(noeud *noeud_courant, noeud *retour[256], int code, int profondeur);

#endif
