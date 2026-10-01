#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#ifdef _WIN32
#include <direct.h>
#define MKDIR(d) _mkdir(d)
#define popen _popen
#define pclose _pclose
#else
#include <sys/stat.h>
#define MKDIR(d) mkdir(d, 0755)
#endif
#include "tris.h"
#include "mesure.h"

#define N_DEPART 1000            /* première taille ; on double ensuite : 2^n * 1000 */
#define N_MAX_DEFAUT 1000000     /* plus grande taille (modifiable : ./tri 2000000) */
#define LIMITE_QUADRATIQUE 65000 /* au-delà, bulles/sélection/insertion sont trop lents */
#define DOSSIER "resultats"

typedef struct {
    const char *nom;
    fonction_tri tri;
    int quadratique;
} Algo;

static const Algo ALGOS[] = {
    {"bulles",    tri_bulles,    1},
    {"selection", tri_selection, 1},
    {"insertion", tri_insertion, 1},
    {"fusion",    tri_fusion,    0},
    {"rapide",    tri_rapide,    0},
    {"tas",       tri_tas,       0},
};
#define NB_ALGOS ((int)(sizeof(ALGOS) / sizeof(ALGOS[0])))

/* Plus N est grand, moins on répète (temps total raisonnable) */
static int nb_repetitions(int n) {
    if (n <= 16000) return 10;
    if (n <= 128000) return 5;
    return 3;
}

/* Trace un graphique PNG par instance, via un pipe vers gnuplot */
static void tracer(TypeInstance inst) {
    FILE *gp = popen("gnuplot", "w");
    if (!gp) {
        fprintf(stderr, "gnuplot introuvable : graphiques non generes\n");
        return;
    }
    const char *ni = nom_instance(inst);

    fprintf(gp, "set terminal png size 1000,600\n");
    fprintf(gp, "set output '%s/graphe_%s.png'\n", DOSSIER, ni);
    fprintf(gp, "set title 'Comparaison des tris - instance : %s'\n", ni);
    fprintf(gp, "set xlabel 'Taille du probleme N (nombre d elements)'\n");
    fprintf(gp, "set ylabel 'Temps moyen (s)'\n");
    fprintf(gp, "set logscale xy\nset grid\nset key left top\n");

    fprintf(gp, "plot ");
    for (int a = 0; a < NB_ALGOS; a++) {
        fprintf(gp, "'%s/%s_%s.dat' using 1:2 with linespoints title '%s'%s",
                DOSSIER, ALGOS[a].nom, ni, ALGOS[a].nom,
                a < NB_ALGOS - 1 ? ", " : "\n");
    }
    pclose(gp);
}

int main(int argc, char *argv[]) {
    int n_max = (argc > 1) ? atoi(argv[1]) : N_MAX_DEFAUT;
    if (n_max < N_DEPART) n_max = N_DEPART;

    MKDIR(DOSSIER);

    for (int i = 0; i < NB_INSTANCES; i++) {
        TypeInstance inst = (TypeInstance)i;
        printf("=== Instance : %s ===\n", nom_instance(inst));

        /* un fichier .dat par algorithme, vidé au départ */
        FILE *f[NB_ALGOS];
        for (int a = 0; a < NB_ALGOS; a++) {
            char chemin[128];
            snprintf(chemin, sizeof chemin, "%s/%s_%s.dat",
                     DOSSIER, ALGOS[a].nom, nom_instance(inst));
            f[a] = fopen(chemin, "w");
            if (!f[a]) { perror(chemin); return 1; }
        }

        for (int n = N_DEPART; n <= n_max; n *= 2) {
            int *src = malloc((size_t)n * sizeof(int));   /* tableau dynamique */
            if (!src) { fprintf(stderr, "malloc echoue (n = %d)\n", n); return 1; }
            generer_instance(src, n, inst);

            int repet = nb_repetitions(n);
            for (int a = 0; a < NB_ALGOS; a++) {
                if (ALGOS[a].quadratique && n > LIMITE_QUADRATIQUE) continue;

                double t = mesurer(ALGOS[a].tri, src, n, repet);
                fprintf(f[a], "%d %.6f\n", n, t);
                fflush(f[a]);
                printf("  N = %8d  %-10s %10.6f s\n", n, ALGOS[a].nom, t);
            }
            free(src);
        }

        for (int a = 0; a < NB_ALGOS; a++) fclose(f[a]);
        tracer(inst);
    }

    printf("Termine. Resultats et graphiques dans %s/\n", DOSSIER);
    return 0;
}