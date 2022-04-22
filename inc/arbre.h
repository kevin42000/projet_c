#ifndef DEF_ARBRE
#define DEF_ARBRE

typedef struct noeud{
  char caractere;
  int occurence;
  char codage[32];
  int nb_bits;
  struct noeud gauche;
  struct noeud droit;
}noeud;

noeud *creation_noeud[256](int tab[256]);

noeud * creer_feuille(int *tab, int index);

void parcours_tableau(noeud *arbre_huffman[256], int *x, int *y);

#endif
