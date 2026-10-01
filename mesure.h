#ifndef MESURE_H
#define MESURE_H

typedef void (*fonction_tri)(int *, int);

typedef enum {
    ALEATOIRE,  /* valeurs aléatoires */
    TRIE,       /* déjà trié (meilleur cas pour bulles/insertion) */
    INVERSE,    /* trié en ordre décroissant (pire cas) */
    DOUBLONS,   /* beaucoup de valeurs répétées (100 valeurs distinctes) */
    NB_INSTANCES
} TypeInstance;

const char *nom_instance(TypeInstance type);

/* Remplit t (taille n) selon le type d'instance */
void generer_instance(int *t, int n, TypeInstance type);

/* Temps moyen (en secondes) d'un tri sur `repet` exécutions.
   Chaque exécution repart d'une copie de src. */
double mesurer(fonction_tri tri, const int *src, int n, int repet);

#endif