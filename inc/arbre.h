#ifndef DEF_ARBRE
#define DEF_ARBRE

typedef struct noeud{
  char initial;
  int occurence;
  int codage;
  int nb_bits;
  struct noeud gauche;
  struct noeud droit;
}noeud;

typedef noeud *arbre;

#endif
