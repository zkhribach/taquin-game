#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <SDL.h>
#include <SDL_ttf.h>
#include "../Headers/functions.h"
#include "../Headers/constante.h"

#define WINDOW_WIDTH 1000
#define WINDOW_HEIGHT 1000

#define FONT_PATH "../TTF/the_genz.ttf"
#define FONT_PATH1 "../TTF/digital-7.ttf"



enum GameState {
    START,
    CREDIT,
    MENU,
    GAME,
    WIN,
    QUIT
};

int game_loop(int n, int g[100][100], SDL_Renderer *renderer, TTF_Font *font, int *game_state) {
    SDL_Event event;
    start_time = SDL_GetTicks();

    while (1) {
        SDL_PollEvent(&event);
        if (event.type == SDL_QUIT ) {
            return QUIT;
        } else if (event.key.keysym.sym == SDLK_ESCAPE) {
            return START;
        }

        process_input(&event, g, n);
        elapsed_time = (SDL_GetTicks() - start_time) / 1000;
        if (check_win(g, n)) {
            game_won = 1;
            update_timer(start_time, &elapsed_time);
            return WIN;
        }

        render_update(g, n, start_time, &elapsed_time, renderer, font, game_won);

        SDL_Delay(1000 / 60);
    }
    return GAME;
}


void sauvegarder_score(const char* nom, Uint32 *elapsed_time) {
    FILE* fichier = fopen("scores.txt", "a");
    if (fichier == NULL) {
        printf("Erreur lors de l'ouverture du fichier des scores\n");
    }

    fprintf(fichier, "%s,%d s \n", nom, *elapsed_time);
    fclose(fichier);
}

void update_timer(Uint32 start_time, Uint32 *elapsed_time) {
    static int score_saved = 0;
    if (!score_saved){
        sauvegarder_score("Anonymous", elapsed_time);
        score_saved = 1;
    }
}

char* afficher_meilleur_score() {
    FILE* fichier = fopen("scores.txt", "r");
    if (fichier == NULL) {
        printf("Aucun score enregistré\n");
        return 0;
    }
    Score scores[100];
    int nb_scores = 0;
    char ligne[256];

    while (fgets(ligne, sizeof(ligne), fichier) && nb_scores < 100) {
        Score s;
        sscanf(ligne, "%[^,],%d s", s.nom, &s.temps);
        scores[nb_scores++] = s;
    }

    fclose(fichier);

    if (nb_scores == 0) {
        printf("Aucun score enregistré\n");
        return 0;
    }

    for (int i = 0; i < nb_scores - 1; i++) {
        for (int j = 0; j < nb_scores - i - 1; j++) {
            if (scores[j].temps > scores[j + 1].temps) {
                Score temp = scores[j];
                scores[j] = scores[j + 1];
                scores[j + 1] = temp;
            }
        }
    }

    static char result[256];

    sprintf(result, " Best: %d s", scores[0].temps);
    return result;
}

int random_(int n) {
    return (rand() % (n - 1)) + 1;
}

void tableau_vide(int chaine[100][100], int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            chaine[i][j] = 0;
        }
    }
}

void tableau_rand(int chaine[100][100], int n) {
    int taille = n * n;
    int a;
    int used[taille];
    for (int i = 0; i < taille; i++) {
        used[i] = 0;
    }
    used[0] = 1;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (i == n - 1 && j == n - 1) {
                continue;
            }
            do {
                a = random_(taille);
            } while (used[a] == 1);
            chaine[i][j] = a;
            used[a] = 1;
        }
    }
}

int compter_inversions(int grid[100][100], int N) {
    int taille = N * N;
    int tableau_1D[taille];
    int index = 0;

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            tableau_1D[index++] = grid[i][j];
        }
    }

    int inversions = 0;

    for (int i = 0; i < taille - 1; i++) {
        for (int j = i + 1; j < taille; j++) {
            if (tableau_1D[i] > tableau_1D[j] && tableau_1D[j] != 0) {
                inversions++;
            }
        }
    }

    return inversions;
}

int position_case_vide(int grid[100][100], int N) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (grid[i][j] == 0) {
                return N - i;
            }
        }
    }
    return -1;
}

int est_solvable(int grid[100][100], int N) {
    int inversions = compter_inversions(grid, N);
    int ligne_vide = position_case_vide(grid, N);

    if (N % 2 == 1) {
        return (inversions % 2 == 0);
    } else {
        return ((inversions % 2 == 0) == (ligne_vide % 2 == 1));
    }
}

void setup(int n, int g[100][100], int y[100][100]) {
    tableau_vide(g, n);
    tableau_rand(g, n);
    do {
        tableau_rand(g, n);
    } while (!est_solvable(g, n));

    int c = 1;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            y[i][j] = c++;
        }
    }
    y[n - 1][n - 1] = 0;
}

int check_win(int grid[][100], int n) {
    int num = 1;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (i == n - 1 && j == n - 1) return 1;
            if (grid[i][j] != num++) return 0;
        }
    }
    return 1;
}

Index detect_index(int x, int t[][100], int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (t[i][j] == x) {
                Index index = {i, j};
                return index;
            }
        }
    }
    Index invalid = {-1, -1};
    return invalid;
}

void swap(int g[][100], int empty_value, int n) {
    Index empty = detect_index(0, g, n);
    Index target = detect_index(empty_value, g, n);

    if (abs(empty.i - target.i) == 1 && empty.j == target.j || abs(empty.j - target.j) == 1 && empty.i == target.i) {
        int temp = g[empty.i][empty.j];
        g[empty.i][empty.j] = g[target.i][target.j];
        g[target.i][target.j] = temp;
    }
}
