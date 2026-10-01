#ifndef TRIS_H
#define TRIS_H

/* Tous les tris ont la même signature : tri en place d'un tableau de n entiers */
void tri_bulles(int *t, int n);
void tri_selection(int *t, int n);
void tri_insertion(int *t, int n);
void tri_fusion(int *t, int n);
void tri_rapide(int *t, int n);
void tri_tas(int *t, int n);

/* Retourne 1 si le tableau est trié par ordre croissant, 0 sinon */
int est_trie(const int *t, int n);

#endif