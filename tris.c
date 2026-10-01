#include <stdlib.h>
#include <string.h>
#include "tris.h"

static void echanger(int *a, int *b) {
    int tmp = *a;
    *a = *b;
    *b = tmp;
}

int est_trie(const int *t, int n) {
    for (int i = 1; i < n; i++)
        if (t[i - 1] > t[i]) return 0;
    return 1;
}

/* ---------- 1. Tri à bulles : O(n^2), O(n) si déjà trié ---------- */
void tri_bulles(int *t, int n) {
    for (int i = 0; i < n - 1; i++) {
        int echange = 0;
        for (int j = 0; j < n - 1 - i; j++) {
            if (t[j] > t[j + 1]) {
                echanger(&t[j], &t[j + 1]);
                echange = 1;
            }
        }
        if (!echange) break; /* déjà trié */
    }
}

/* ---------- 2. Tri par sélection : O(n^2) dans tous les cas ---------- */
void tri_selection(int *t, int n) {
    for (int i = 0; i < n - 1; i++) {
        int min = i;
        for (int j = i + 1; j < n; j++)
            if (t[j] < t[min]) min = j;
        if (min != i) echanger(&t[i], &t[min]);
    }
}

/* ---------- 3. Tri par insertion : O(n^2), O(n) si déjà trié ---------- */
void tri_insertion(int *t, int n) {
    for (int i = 1; i < n; i++) {
        int cle = t[i];
        int j = i - 1;
        while (j >= 0 && t[j] > cle) {
            t[j + 1] = t[j];
            j--;
        }
        t[j + 1] = cle;
    }
}

/* ---------- 4. Tri fusion : O(n log n), mémoire O(n) ---------- */
static void fusion_rec(int *t, int *tmp, int g, int d) { /* intervalle [g, d[ */
    if (d - g < 2) return;
    int m = g + (d - g) / 2;
    fusion_rec(t, tmp, g, m);
    fusion_rec(t, tmp, m, d);

    int i = g, j = m, k = g;
    while (i < m && j < d)
        tmp[k++] = (t[i] <= t[j]) ? t[i++] : t[j++];
    while (i < m) tmp[k++] = t[i++];
    while (j < d) tmp[k++] = t[j++];
    memcpy(t + g, tmp + g, (size_t)(d - g) * sizeof(int));
}

void tri_fusion(int *t, int n) {
    if (n < 2) return;
    int *tmp = malloc((size_t)n * sizeof(int));
    if (!tmp) return;
    fusion_rec(t, tmp, 0, n);
    free(tmp);
}

/* ---------- 5. Tri rapide : O(n log n) en moyenne, O(n^2) au pire ----------
   Pivot au milieu (évite le pire cas sur tableaux triés) et récursion
   sur la plus petite partie seulement (pile limitée à O(log n)). */
static void rapide_rec(int *t, int bas, int haut) {
    while (bas < haut) {
        int pivot = t[bas + (haut - bas) / 2];
        int i = bas, j = haut;
        while (i <= j) {
            while (t[i] < pivot) i++;
            while (t[j] > pivot) j--;
            if (i <= j) {
                echanger(&t[i], &t[j]);
                i++;
                j--;
            }
        }
        if (j - bas < haut - i) {
            rapide_rec(t, bas, j);
            bas = i;
        } else {
            rapide_rec(t, i, haut);
            haut = j;
        }
    }
}

void tri_rapide(int *t, int n) {
    rapide_rec(t, 0, n - 1);
}

/* ---------- 6. Tri par tas : O(n log n) dans tous les cas, en place ---------- */
static void tamiser(int *t, int racine, int n) {
    while (2 * racine + 1 < n) {
        int fils = 2 * racine + 1;
        if (fils + 1 < n && t[fils] < t[fils + 1]) fils++;
        if (t[racine] >= t[fils]) return;
        echanger(&t[racine], &t[fils]);
        racine = fils;
    }
}

void tri_tas(int *t, int n) {
    for (int i = n / 2 - 1; i >= 0; i--)
        tamiser(t, i, n);
    for (int fin = n - 1; fin > 0; fin--) {
        echanger(&t[0], &t[fin]);
        tamiser(t, 0, fin);
    }
}