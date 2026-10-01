#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#ifdef _WIN32
#include <windows.h>
#endif
#include "mesure.h"
#include "tris.h"

/* Générateur xorshift : même suite sur tous les systèmes (résultats reproductibles) */
static unsigned long long etat = 88172645463325252ULL;

static unsigned int alea(void) {
    etat ^= etat << 13;
    etat ^= etat >> 7;
    etat ^= etat << 17;
    return (unsigned int)(etat >> 32);
}

const char *nom_instance(TypeInstance type) {
    switch (type) {
        case ALEATOIRE: return "aleatoire";
        case TRIE:      return "trie";
        case INVERSE:   return "inverse";
        case DOUBLONS:  return "doublons";
        default:        return "inconnu";
    }
}

void generer_instance(int *t, int n, TypeInstance type) {
    for (int i = 0; i < n; i++) {
        switch (type) {
            case ALEATOIRE: t[i] = (int)(alea() >> 1); break;
            case TRIE:      t[i] = i;                  break;
            case INVERSE:   t[i] = n - i;              break;
            case DOUBLONS:  t[i] = (int)(alea() % 100); break;
            default:        t[i] = 0;                  break;
        }
    }
}

/* Horloge en secondes (haute résolution, Windows et Linux/macOS) */
static double maintenant(void) {
#ifdef _WIN32
    LARGE_INTEGER freq, cpt;
    QueryPerformanceFrequency(&freq);
    QueryPerformanceCounter(&cpt);
    return (double)cpt.QuadPart / (double)freq.QuadPart;
#else
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (double)ts.tv_sec + (double)ts.tv_nsec / 1e9;
#endif
}

double mesurer(fonction_tri tri, const int *src, int n, int repet) {
    int *t = malloc((size_t)n * sizeof(int));
    if (!t) return -1.0;

    double total = 0.0;
    for (int r = 0; r < repet; r++) {
        memcpy(t, src, (size_t)n * sizeof(int)); /* copie hors chrono */

        double debut = maintenant();
        tri(t, n);
        total += maintenant() - debut;

        if (!est_trie(t, n))
            fprintf(stderr, "ERREUR : tableau non trie (n = %d)\n", n);
    }
    free(t);
    return total / repet; /* moyenne = lissage */
}