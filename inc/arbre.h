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

#endif
