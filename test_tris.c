#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tris.h"

typedef void (*fonction_tri)(int *, int);

int main(void) {
    const char *noms[] = {"bulles", "selection", "insertion", "fusion", "rapide", "tas"};
    fonction_tri tris[] = {tri_bulles, tri_selection, tri_insertion,
                           tri_fusion, tri_rapide, tri_tas};
    int nb = 6;
    int tailles[] = {0, 1, 2, 10, 1000};

    srand(42);
    for (int a = 0; a < nb; a++) {
        int ok = 1;
        for (int s = 0; s < 5; s++) {
            int n = tailles[s];
            int *t = malloc((n > 0 ? n : 1) * sizeof(int));
            for (int i = 0; i < n; i++) t[i] = rand() % 100; /* avec doublons */
            tris[a](t, n);
            if (!est_trie(t, n)) ok = 0;
            free(t);
        }
        printf("tri %-10s : %s\n", noms[a], ok ? "OK" : "ERREUR");
    }
    return 0;
}