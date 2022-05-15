#ifndef DEF_ARBRE
#define DEF_ARBRE
#define MAX_CHAR 257
#define MAX_PROF 33
#define MAX_CHEMIN 4097

typedef struct noeud{
  char caractere;
  int occurence;
  char codage[MAX_PROF];/* Codage binaire */
  int nb_bits;/* le nombre de bits utilisé par le codage */
  struct noeud *gauche;
  struct noeud *droit;
}noeud;

/**
 * @brief Crée une feuille à partir de la table d'occurrence pour le caractère index
 * 
 * @param tab table d'occurrence
 * @param index le caractère
 * @return noeud* 
 */
noeud * creer_feuille(int *tab, int index);


/**
 * @brief Récupère l'indice des deux plus faibles occurrences de l'arbre
 * 
 * @param arbre_huffman l'arbre
 * @param taille la taille
 * @param x le plus petit
 * @param y le second plus petit
 */
void parcours_tableau(noeud *arbre_huffman[MAX_CHAR], int taille, int *x, int *y);

/**
 * @brief Crée les noeuds selon les deux plus faibles occurrences
 * 
 * @param tab l'arbre
 * @param taille sa taille
 */
void creer_noeud(noeud *tab[], int taille);

/**
 * @brief Teste si le noeud_courant est une feuille
 * 
 * @param noeud_courant le noeud à tester
 * @return int 0 = faux 1 = vrai
 */
int est_feuille(noeud *noeud_courant);

/**
 * @brief crée le code binaire des caractères de l'arbre
 * 
 * @param noeud_courant noeud à traiter
 * @param retour arbre de retour
 * @param code code binaire
 * @param profondeur la profondeur de l'arbre
 */
void creer_code(noeud *noeud_courant, noeud *retour[MAX_CHAR], char code[MAX_PROF], int profondeur);

#endif
