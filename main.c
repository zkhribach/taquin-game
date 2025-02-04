#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <SDL.h>
#include <SDL_ttf.h>
#include "functions.h"
#include "constante.h"

#define WINDOW_WIDTH 1000
#define WINDOW_HEIGHT 1000

#define FONT_PATH "./the_genz.ttf"
#define FONT_PATH1 "./digital-7.ttf"

enum GameState {
    START,
    CREDIT,
    MENU,
    GAME,
    WIN,
    QUIT
};








int main(int argc, char *argv[]) {
    srand(time(0));

    if (initialize_window() != 0) {
        return 1;
    }

    int n;
    int g[100][100];
    int y[100][100];

    int game_state = START;

    SDL_Event event;
    while (1) {
        SDL_PollEvent(&event);
        if (event.type == SDL_QUIT) {
            break;
        }
        if (game_state == START) {
            process_start_input(&event, &selected_option, &game_state);
            render_start(renderer, font, selected_option);
            game_won = 0;
        }else if (game_state == MENU) {
            process_menu_input(&event, &selected_option, &game_state);
            render_menu(renderer, font, selected_option);
        } else if (game_state == GAME) {
            n = 3 + selected_option;
            setup(n, g, y);
            game_state = game_loop(n, g, renderer, font1, &game_state);
        } else if (game_state == QUIT){
            break;
        } else if (game_state == CREDIT){
            process_credit_input(&event, &selected_option, &game_state);
            render_credit(renderer, font, selected_option);
        }else if (game_state == WIN){
            process_credit_input(&event, &selected_option, &game_state);
            render_win(renderer, font, selected_option);
        }
        SDL_Delay(1000 / 60);
    }

    destroy_window();
    return 0;
}

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

void render_update(int g[100][100], int n, Uint32 start_time, Uint32 *elapsed_time, SDL_Renderer *renderer, TTF_Font *font, int game_won) {
    char* result[256];
    SDL_RenderClear(renderer);

    SDL_RenderCopy(renderer, BACKGROUND1_texture, NULL, NULL);
    render_grid(g, n, renderer, font);


    char time_text[32];
    sprintf(time_text, "Time: %d s", *elapsed_time);

    SDL_Color white = {255, 255, 255, 255};
    SDL_Surface* surface = TTF_RenderText_Solid(font, time_text, white);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_Rect dst = {WINDOW_WIDTH - surface->w - 10, 20, surface->w, surface->h};
    SDL_RenderCopy(renderer, texture, NULL, &dst);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);

    *result = afficher_meilleur_score();
    SDL_Surface* surface1 = TTF_RenderText_Solid(font, *result, white);
    SDL_Texture* texture1 = SDL_CreateTextureFromSurface(renderer, surface1);
    SDL_Rect dst1 = {WINDOW_WIDTH - surface1->w - 10, 0, surface1->w, surface1->h};
    SDL_RenderCopy(renderer, texture1, NULL, &dst1);
    SDL_FreeSurface(surface1);
    SDL_DestroyTexture(texture1);


    char escape[32] = "<-- Esc";
    SDL_Surface* surface2 = TTF_RenderText_Solid(font, escape, white);
    SDL_Texture* texture2 = SDL_CreateTextureFromSurface(renderer, surface2);
    SDL_Rect dst2 = {0, 0, surface2->w, surface2->h};
    SDL_RenderCopy(renderer, texture2, NULL, &dst2);
    SDL_FreeSurface(surface2);
    SDL_DestroyTexture(texture2);

    SDL_RenderPresent(renderer);
}

void render_grid(int g[100][100], int n, SDL_Renderer *renderer, TTF_Font *font) {
    int tile_width = ( WINDOW_WIDTH  ) / n;
    int tile_height = (WINDOW_HEIGHT - 40 ) / n;
    int margin = 10;
    int top = 20;

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (g[i][j] != 0) {
                SDL_Rect tile = {j * tile_width, i * tile_height + top, tile_width, tile_height};

                SDL_Rect inner_tile = {tile.x + margin, tile.y + margin + top, tile_width - 2 * margin, tile_height - 2 * margin};

                switch (g[i][j]) {
                    case 1: SDL_RenderCopy(renderer, tile1_texture, NULL, &inner_tile); break;
                    case 2: SDL_RenderCopy(renderer, tile2_texture, NULL, &inner_tile); break;
                    case 3: SDL_RenderCopy(renderer, tile3_texture, NULL, &inner_tile); break;
                    case 4: SDL_RenderCopy(renderer, tile4_texture, NULL, &inner_tile); break;
                    case 5: SDL_RenderCopy(renderer, tile5_texture, NULL, &inner_tile); break;
                    case 6: SDL_RenderCopy(renderer, tile6_texture, NULL, &inner_tile); break;
                    case 7: SDL_RenderCopy(renderer, tile7_texture, NULL, &inner_tile); break;
                    case 8: SDL_RenderCopy(renderer, tile8_texture, NULL, &inner_tile); break;
                    case 9: SDL_RenderCopy(renderer, tile9_texture, NULL, &inner_tile); break;
                    case 10: SDL_RenderCopy(renderer, tile10_texture, NULL, &inner_tile); break;
                    case 11: SDL_RenderCopy(renderer, tile11_texture, NULL, &inner_tile); break;
                    case 12: SDL_RenderCopy(renderer, tile12_texture, NULL, &inner_tile); break;
                    case 13: SDL_RenderCopy(renderer, tile13_texture, NULL, &inner_tile); break;
                    case 14: SDL_RenderCopy(renderer, tile14_texture, NULL, &inner_tile); break;
                    case 15: SDL_RenderCopy(renderer, tile15_texture, NULL, &inner_tile); break;
                    case 16: SDL_RenderCopy(renderer, tile16_texture, NULL, &inner_tile); break;
                    case 17: SDL_RenderCopy(renderer, tile17_texture, NULL, &inner_tile); break;
                    case 18: SDL_RenderCopy(renderer, tile18_texture, NULL, &inner_tile); break;
                    case 19: SDL_RenderCopy(renderer, tile19_texture, NULL, &inner_tile); break;
                    case 20: SDL_RenderCopy(renderer, tile20_texture, NULL, &inner_tile); break;
                    case 21: SDL_RenderCopy(renderer, tile21_texture, NULL, &inner_tile); break;
                    case 22: SDL_RenderCopy(renderer, tile22_texture, NULL, &inner_tile); break;
                    case 23: SDL_RenderCopy(renderer, tile23_texture, NULL, &inner_tile); break;
                    case 24: SDL_RenderCopy(renderer, tile24_texture, NULL, &inner_tile); break;
                    case 25: SDL_RenderCopy(renderer, tile25_texture, NULL, &inner_tile); break;
                }

            }
        }
    }
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

int initialize_window(void) {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        printf("SDL Initialization failed\n");
        return -1;
    }

    window = SDL_CreateWindow("Taquin Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    if (!window) {
        printf("Window creation failed\n");
        SDL_Quit();
        return -1;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        printf("Renderer creation failed\n");
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }

    if (TTF_Init() == -1) {
        printf("TTF Initialization failed\n");
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }

    font = TTF_OpenFont(FONT_PATH, 24);
    if (!font) {
        printf("Failed to load font: %s\n", TTF_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }

    font1 = TTF_OpenFont(FONT_PATH1, 24);
    if (!font) {
        printf("Failed to load font1: %s\n", TTF_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }

    SDL_Surface* background_surface = SDL_LoadBMP("taquin.bmp");
    if (!background_surface) {
        printf("Failed to load background image: %s\n", SDL_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        TTF_CloseFont(font);
        TTF_Quit();
        SDL_Quit();
        return -1;
    }
    background_texture = SDL_CreateTextureFromSurface(renderer, background_surface);
    SDL_FreeSurface(background_surface);
    if (!background_texture) {
        printf("Failed to create texture from background image: %s\n", SDL_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        TTF_CloseFont(font);
        TTF_Quit();
        SDL_Quit();
        return -1;
    }
    SDL_Surface* credit_surface = SDL_LoadBMP("credit.bmp");
    if (!credit_surface) {
        printf("Failed to load credit.bmp: %s\n", SDL_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        TTF_CloseFont(font);
        TTF_Quit();
        SDL_Quit();
        return -1;
    }
    credit_texture = SDL_CreateTextureFromSurface(renderer, credit_surface);
    SDL_FreeSurface(credit_surface);
    if (!credit_texture) {
        printf("Failed to create texture from credit.bmp: %s\n", SDL_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        TTF_CloseFont(font);
        TTF_Quit();
        SDL_Quit();
        return -1;
    }
    SDL_Surface* tile1_surface = SDL_LoadBMP("tile1.bmp");
    if (!tile1_surface) {
        printf("Failed to load tile1.bmp: %s\n", SDL_GetError());
        return -1;
    }
    tile1_texture = SDL_CreateTextureFromSurface(renderer, tile1_surface);
    SDL_FreeSurface(tile1_surface);
    if (!tile1_texture) {
        printf("Failed to create texture from tile1.bmp: %s\n", SDL_GetError());
        return -1;
    }

    SDL_Surface* tile2_surface = SDL_LoadBMP("tile2.bmp");
    if (!tile2_surface) {
        printf("Failed to load tile2.bmp: %s\n", SDL_GetError());
        return -1;
    }
    tile2_texture = SDL_CreateTextureFromSurface(renderer, tile2_surface);
    SDL_FreeSurface(tile2_surface);
    if (!tile2_texture) {
        printf("Failed to create texture from tile2.bmp: %s\n", SDL_GetError());
        return -1;
    }

    SDL_Surface* tile3_surface = SDL_LoadBMP("tile3.bmp");
    if (!tile3_surface) {
        printf("Failed to load tile3.bmp: %s\n", SDL_GetError());
        return -1;
    }
    tile3_texture = SDL_CreateTextureFromSurface(renderer, tile3_surface);
    SDL_FreeSurface(tile3_surface);
    if (!tile3_texture) {
        printf("Failed to create texture from tile3.bmp: %s\n", SDL_GetError());
        return -1;
    }

    SDL_Surface* tile4_surface = SDL_LoadBMP("tile4.bmp");
    if (!tile4_surface) {
        printf("Failed to load tile4.bmp: %s\n", SDL_GetError());
        return -1;
    }
    tile4_texture = SDL_CreateTextureFromSurface(renderer, tile4_surface);
    SDL_FreeSurface(tile4_surface);
    if (!tile4_texture) {
        printf("Failed to create texture from tile4.bmp: %s\n", SDL_GetError());
        return -1;
    }

    SDL_Surface* tile5_surface = SDL_LoadBMP("tile5.bmp");
    if (!tile5_surface) {
        printf("Failed to load tile5.bmp: %s\n", SDL_GetError());
        return -1;
    }
    tile5_texture = SDL_CreateTextureFromSurface(renderer, tile5_surface);
    SDL_FreeSurface(tile5_surface);
    if (!tile5_texture) {
        printf("Failed to create texture from tile5.bmp: %s\n", SDL_GetError());
        return -1;
    }

    SDL_Surface* tile6_surface = SDL_LoadBMP("tile6.bmp");
    if (!tile6_surface) {
        printf("Failed to load tile6.bmp: %s\n", SDL_GetError());
        return -1;
    }
    tile6_texture = SDL_CreateTextureFromSurface(renderer, tile6_surface);
    SDL_FreeSurface(tile6_surface);
    if (!tile6_texture) {
        printf("Failed to create texture from tile6.bmp: %s\n", SDL_GetError());
        return -1;
    }

    SDL_Surface* tile7_surface = SDL_LoadBMP("tile7.bmp");
    if (!tile7_surface) {
        printf("Failed to load tile7.bmp: %s\n", SDL_GetError());
        return -1;
    }
    tile7_texture = SDL_CreateTextureFromSurface(renderer, tile7_surface);
    SDL_FreeSurface(tile7_surface);
    if (!tile7_texture) {
        printf("Failed to create texture from tile7.bmp: %s\n", SDL_GetError());
        return -1;
    }

    SDL_Surface* tile8_surface = SDL_LoadBMP("tile8.bmp");
    if (!tile8_surface) {
        printf("Failed to load tile8.bmp: %s\n", SDL_GetError());
        return -1;
    }
    tile8_texture = SDL_CreateTextureFromSurface(renderer, tile8_surface);
    SDL_FreeSurface(tile8_surface);
    if (!tile8_texture) {
        printf("Failed to create texture from tile8.bmp: %s\n", SDL_GetError());
        return -1;
    }

    SDL_Surface* tile9_surface = SDL_LoadBMP("tile9.bmp");
    if (!tile9_surface) {
        printf("Failed to load tile9.bmp: %s\n", SDL_GetError());
        return -1;
    }
    tile9_texture = SDL_CreateTextureFromSurface(renderer, tile9_surface);
    SDL_FreeSurface(tile9_surface);
    if (!tile9_texture) {
        printf("Failed to create texture from tile9.bmp: %s\n", SDL_GetError());
        return -1;
    }

    SDL_Surface* tile10_surface = SDL_LoadBMP("tile10.bmp");
    if (!tile10_surface) {
        printf("Failed to load tile10.bmp: %s\n", SDL_GetError());
        return -1;
    }
    tile10_texture = SDL_CreateTextureFromSurface(renderer, tile10_surface);
    SDL_FreeSurface(tile10_surface);
    if (!tile10_texture) {
        printf("Failed to create texture from tile10.bmp: %s\n", SDL_GetError());
        return -1;
    }

    SDL_Surface* tile11_surface = SDL_LoadBMP("tile11.bmp");
    if (!tile11_surface) {
        printf("Failed to load tile11.bmp: %s\n", SDL_GetError());
        return -1;
    }
    tile11_texture = SDL_CreateTextureFromSurface(renderer, tile11_surface);
    SDL_FreeSurface(tile11_surface);
    if (!tile11_texture) {
        printf("Failed to create texture from tile11.bmp: %s\n", SDL_GetError());
        return -1;
    }

    SDL_Surface* tile12_surface = SDL_LoadBMP("tile12.bmp");
    if (!tile12_surface) {
        printf("Failed to load tile12.bmp: %s\n", SDL_GetError());
        return -1;
    }
    tile12_texture = SDL_CreateTextureFromSurface(renderer, tile12_surface);
    SDL_FreeSurface(tile12_surface);
    if (!tile12_texture) {
        printf("Failed to create texture from tile12.bmp: %s\n", SDL_GetError());
        return -1;
    }

    SDL_Surface* tile13_surface = SDL_LoadBMP("tile13.bmp");
    if (!tile13_surface) {
        printf("Failed to load tile13.bmp: %s\n", SDL_GetError());
        return -1;
    }
    tile13_texture = SDL_CreateTextureFromSurface(renderer, tile13_surface);
    SDL_FreeSurface(tile13_surface);
    if (!tile13_texture) {
        printf("Failed to create texture from tile13.bmp: %s\n", SDL_GetError());
        return -1;
    }

    SDL_Surface* tile14_surface = SDL_LoadBMP("tile14.bmp");
    if (!tile14_surface) {
        printf("Failed to load tile14.bmp: %s\n", SDL_GetError());
        return -1;
    }
    tile14_texture = SDL_CreateTextureFromSurface(renderer, tile14_surface);
    SDL_FreeSurface(tile14_surface);
    if (!tile14_texture) {
        printf("Failed to create texture from tile14.bmp: %s\n", SDL_GetError());
        return -1;
    }

    SDL_Surface* tile15_surface = SDL_LoadBMP("tile15.bmp");
    if (!tile15_surface) {
        printf("Failed to load tile15.bmp: %s\n", SDL_GetError());
        return -1;
    }
    tile15_texture = SDL_CreateTextureFromSurface(renderer, tile15_surface);
    SDL_FreeSurface(tile15_surface);
    if (!tile15_texture) {
        printf("Failed to create texture from tile15.bmp: %s\n", SDL_GetError());
        return -1;
    }

    SDL_Surface* tile16_surface = SDL_LoadBMP("tile16.bmp");
    if (!tile16_surface) {
        printf("Failed to load tile16.bmp: %s\n", SDL_GetError());
        return -1;
    }
    tile16_texture = SDL_CreateTextureFromSurface(renderer, tile16_surface);
    SDL_FreeSurface(tile16_surface);
    if (!tile16_texture) {
        printf("Failed to create texture from tile16.bmp: %s\n", SDL_GetError());
        return -1;
    }

    SDL_Surface* tile17_surface = SDL_LoadBMP("tile17.bmp");
    if (!tile17_surface) {
        printf("Failed to load tile17.bmp: %s\n", SDL_GetError());
        return -1;
    }
    tile17_texture = SDL_CreateTextureFromSurface(renderer, tile17_surface);
    SDL_FreeSurface(tile17_surface);
    if (!tile17_texture) {
        printf("Failed to create texture from tile17.bmp: %s\n", SDL_GetError());
        return -1;
    }

    SDL_Surface* tile18_surface = SDL_LoadBMP("tile18.bmp");
    if (!tile18_surface) {
        printf("Failed to load tile18.bmp: %s\n", SDL_GetError());
        return -1;
    }
    tile18_texture = SDL_CreateTextureFromSurface(renderer, tile18_surface);
    SDL_FreeSurface(tile18_surface);
    if (!tile18_texture) {
        printf("Failed to create texture from tile18.bmp: %s\n", SDL_GetError());
        return -1;
    }

    SDL_Surface* tile19_surface = SDL_LoadBMP("tile19.bmp");
    if (!tile19_surface) {
        printf("Failed to load tile19.bmp: %s\n", SDL_GetError());
        return -1;
    }
    tile19_texture = SDL_CreateTextureFromSurface(renderer, tile19_surface);
    SDL_FreeSurface(tile19_surface);
    if (!tile19_texture) {
        printf("Failed to create texture from tile19.bmp: %s\n", SDL_GetError());
        return -1;
    }

    SDL_Surface* tile20_surface = SDL_LoadBMP("tile20.bmp");
    if (!tile20_surface) {
        printf("Failed to load tile20.bmp: %s\n", SDL_GetError());
        return -1;
    }
    tile20_texture = SDL_CreateTextureFromSurface(renderer, tile20_surface);
    SDL_FreeSurface(tile20_surface);
    if (!tile20_texture) {
        printf("Failed to create texture from tile20.bmp: %s\n", SDL_GetError());
        return -1;
    }

    SDL_Surface* tile21_surface = SDL_LoadBMP("tile21.bmp");
    if (!tile21_surface) {
        printf("Failed to load tile21.bmp: %s\n", SDL_GetError());
        return -1;
    }
    tile21_texture = SDL_CreateTextureFromSurface(renderer, tile21_surface);
    SDL_FreeSurface(tile21_surface);
    if (!tile21_texture) {
        printf("Failed to create texture from tile21.bmp: %s\n", SDL_GetError());
        return -1;
    }

    SDL_Surface* tile22_surface = SDL_LoadBMP("tile22.bmp");
    if (!tile22_surface) {
        printf("Failed to load tile22.bmp: %s\n", SDL_GetError());
        return -1;
    }
    tile22_texture = SDL_CreateTextureFromSurface(renderer, tile22_surface);
    SDL_FreeSurface(tile22_surface);
    if (!tile22_texture) {
        printf("Failed to create texture from tile22.bmp: %s\n", SDL_GetError());
        return -1;
    }

    SDL_Surface* tile23_surface = SDL_LoadBMP("tile23.bmp");
    if (!tile23_surface) {
        printf("Failed to load tile23.bmp: %s\n", SDL_GetError());
        return -1;
    }
    tile23_texture = SDL_CreateTextureFromSurface(renderer, tile23_surface);
    SDL_FreeSurface(tile23_surface);
    if (!tile23_texture) {
        printf("Failed to create texture from tile23.bmp: %s\n", SDL_GetError());
        return -1;
    }

    SDL_Surface* tile24_surface = SDL_LoadBMP("tile24.bmp");
    if (!tile24_surface) {
        printf("Failed to load tile24.bmp: %s\n", SDL_GetError());
        return -1;
    }
    tile24_texture = SDL_CreateTextureFromSurface(renderer, tile24_surface);
    SDL_FreeSurface(tile24_surface);
    if (!tile24_texture) {
        printf("Failed to create texture from tile24.bmp: %s\n", SDL_GetError());
        return -1;
    }

    SDL_Surface* tile25_surface = SDL_LoadBMP("tile25.bmp");
    if (!tile25_surface) {
        printf("Failed to load tile25.bmp: %s\n", SDL_GetError());
        return -1;
    }
    tile25_texture = SDL_CreateTextureFromSurface(renderer, tile25_surface);
    SDL_FreeSurface(tile25_surface);
    if (!tile25_texture) {
        printf("Failed to create texture from tile25.bmp: %s\n", SDL_GetError());
        return -1;
    }
    SDL_Surface* BACKGROUND1_surface = SDL_LoadBMP("BACKGROUND1.bmp");
    if (!BACKGROUND1_surface) {
        printf("Failed to load BACKGROUND1.bmp: %s\n", SDL_GetError());
        return -1;
    }
    BACKGROUND1_texture = SDL_CreateTextureFromSurface(renderer, BACKGROUND1_surface);
    SDL_FreeSurface(BACKGROUND1_surface);
    if (!BACKGROUND1_texture) {
        printf("Failed to create texture from BACKGROUND1.bmp: %s\n", SDL_GetError());
        return -1;
    }
    SDL_Surface* difficulte_surface = SDL_LoadBMP("difficulte.bmp");
    if (!difficulte_surface) {
        printf("Failed to load BACKGROUND1.bmp: %s\n", SDL_GetError());
        return -1;
    }
    difficulte_texture = SDL_CreateTextureFromSurface(renderer, difficulte_surface);
    SDL_FreeSurface(difficulte_surface);
    if (!difficulte_texture) {
        printf("Failed to create texture from BACKGROUND1.bmp: %s\n", SDL_GetError());
        return -1;
    }
    SDL_Surface* win_surface = SDL_LoadBMP("win.bmp");
    if (!win_surface) {
        printf("Failed to load BACKGROUND1.bmp: %s\n", SDL_GetError());
        return -1;
    }
    win_texture = SDL_CreateTextureFromSurface(renderer, win_surface);
    SDL_FreeSurface(win_surface);
    if (!win_texture) {
        printf("Failed to create texture from BACKGROUND1.bmp: %s\n", SDL_GetError());
        return -1;
    }
    return 0;
}

void process_input(SDL_Event *event, int g[100][100], int n) {
    if (event->type == SDL_KEYDOWN) {
        Index empty = detect_index(0, g, n);
        switch (event->key.keysym.sym) {
            case SDLK_UP:
                if (empty.i < n - 1) {
                    swap(g, g[empty.i + 1][empty.j], n);
                }
                break;
            case SDLK_DOWN:
                if (empty.i > 0) {
                    swap(g, g[empty.i - 1][empty.j], n);
                }
                break;
            case SDLK_LEFT:
                if (empty.j < n - 1) {
                    swap(g, g[empty.i][empty.j + 1], n);
                }
                break;
            case SDLK_RIGHT:
                if (empty.j > 0) {
                    swap(g, g[empty.i][empty.j - 1], n);
                }
                break;
            default:
                break;
        }
    }
}
void process_start_input(SDL_Event *event, int *selected_option, int *menu_state){
    if (event->type == SDL_KEYDOWN) {
        switch (event->key.keysym.sym) {
            case SDLK_DOWN:
                *selected_option = (*selected_option + 1) % 2;
                break;
            case SDLK_UP:
                *selected_option = (*selected_option + 1 ) % 2;
                break;
            case SDLK_RETURN:
                switch (*selected_option) {
                    case 0 : *menu_state = MENU; break;
                    case 1 : *menu_state = CREDIT; break;
                }

                break;
            default:
                break;
        }
    }
}


void process_menu_input(SDL_Event *event, int *selected_option, int *menu_state) {
    if (event->type == SDL_KEYDOWN) {
        switch (event->key.keysym.sym) {
            case SDLK_DOWN:
                *selected_option = (*selected_option + 1) % 3;
                break;
            case SDLK_UP:
                *selected_option = (*selected_option + 2) % 3;
                break;
            case SDLK_RETURN:
                *menu_state = GAME;
                break;
            case SDLK_ESCAPE:
                *menu_state = START;
                break;
            default:
                break;
        }
    }
}

void render_start(SDL_Renderer *renderer, TTF_Font *font, int selected_option) {
    SDL_Color red = {3, 3, 3, 255};
    SDL_Color white = {255, 255, 255, 255};
    SDL_Color yellow = {255, 255, 0, 255};

    const char* options[] = { "Start Game", "Credit" };
    SDL_Color colors[] = { white, white, white };
    colors[selected_option] = yellow;

    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, background_texture, NULL, NULL);
    const char* title = "Press Return to start.";
    SDL_Surface* title_surface = TTF_RenderText_Solid(font, title, red);
    SDL_Texture* title_texture = SDL_CreateTextureFromSurface(renderer, title_surface);
    SDL_Rect title_rect = { WINDOW_WIDTH / 2 - title_surface->w / 2, WINDOW_HEIGHT / 5 , title_surface->w, title_surface->h };
    SDL_RenderCopy(renderer, title_texture, NULL, &title_rect);
    SDL_FreeSurface(title_surface);
    SDL_DestroyTexture(title_texture);

    for (int i = 0; i < 2; i++) {
        SDL_Surface* surface = TTF_RenderText_Solid(font, options[i], colors[i]);
        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_Rect dst = { WINDOW_WIDTH / 2 - surface->w / 2, WINDOW_HEIGHT - (2 * surface->h) - 200 + i * 50, surface->w, surface->h };
        SDL_RenderCopy(renderer, texture, NULL, &dst);
        SDL_FreeSurface(surface);
        SDL_DestroyTexture(texture);
    }
    SDL_RenderPresent(renderer);
}

void process_credit_input(SDL_Event *event, int *selected_option, int *menu_state) {
    if (event->type == SDL_KEYDOWN) {
        switch (event->key.keysym.sym) {
            case SDLK_ESCAPE:
                *menu_state = START;
                break;
            default:
                break;
        }
    }
}
void render_credit(SDL_Renderer *renderer, TTF_Font *font, int selected_option) {

    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, credit_texture, NULL, NULL);



    SDL_RenderPresent(renderer);

}

void render_win(SDL_Renderer *renderer, TTF_Font *font, int selected_option) {

    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, win_texture, NULL, NULL);

    char time_text[32];
    sprintf(time_text, "Time: %d s", elapsed_time);

    SDL_Color white = {255, 255, 255, 255};
    SDL_Surface* surface = TTF_RenderText_Solid(font, time_text, white);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_Rect dst = {(WINDOW_WIDTH - surface->w) / 2, (WINDOW_HEIGHT - surface->h) / 2, surface->w, surface->h};
    SDL_RenderCopy(renderer, texture, NULL, &dst);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);

    SDL_RenderPresent(renderer);

}

void render_menu(SDL_Renderer *renderer, TTF_Font *font, int selected_option) {
    SDL_Color white = {0, 0, 0, 255};
    SDL_Color yellow = {255, 0, 255, 255};

    const char* options[] = { "3x3", "4x4", "5x5" };
    SDL_Color colors[] = { white, white, white };
    colors[selected_option] = yellow;

    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, difficulte_texture, NULL, NULL);
    const char* title = "Select difficulty :";
    SDL_Surface* title_surface = TTF_RenderText_Solid(font, title, white);
    SDL_Texture* title_texture = SDL_CreateTextureFromSurface(renderer, title_surface);
    SDL_Rect title_rect = { WINDOW_WIDTH / 2 - title_surface->w / 2, WINDOW_HEIGHT / 2.5, title_surface->w, title_surface->h };
    SDL_RenderCopy(renderer, title_texture, NULL, &title_rect);
    SDL_FreeSurface(title_surface);
    SDL_DestroyTexture(title_texture);

    char escape[32] = "<-- Esc";
    SDL_Surface* surface2 = TTF_RenderText_Solid(font, escape, white);
    SDL_Texture* texture2 = SDL_CreateTextureFromSurface(renderer, surface2);
    SDL_Rect dst2 = {0, 0, surface2->w, surface2->h};
    SDL_RenderCopy(renderer, texture2, NULL, &dst2);
    SDL_FreeSurface(surface2);
    SDL_DestroyTexture(texture2);

    for (int i = 0; i < 3; i++) {
        SDL_Surface* surface = TTF_RenderText_Solid(font, options[i], colors[i]);
        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_Rect dst = { WINDOW_WIDTH / 2 - surface->w / 2, WINDOW_HEIGHT / 2.2 + i * 40, surface->w, surface->h };
        SDL_RenderCopy(renderer, texture, NULL, &dst);
        SDL_FreeSurface(surface);
        SDL_DestroyTexture(texture);
    }
    SDL_RenderPresent(renderer);
}

void destroy_window(void) {
    SDL_DestroyTexture(BACKGROUND1_texture);
    SDL_DestroyTexture(credit_texture);
    SDL_DestroyTexture(tile1_texture);
    SDL_DestroyTexture(tile2_texture);
    SDL_DestroyTexture(tile3_texture);
    SDL_DestroyTexture(tile4_texture);
    SDL_DestroyTexture(tile5_texture);
    SDL_DestroyTexture(tile6_texture);
    SDL_DestroyTexture(tile7_texture);
    SDL_DestroyTexture(tile8_texture);
    SDL_DestroyTexture(tile9_texture);
    SDL_DestroyTexture(tile10_texture);
    SDL_DestroyTexture(tile11_texture);
    SDL_DestroyTexture(tile12_texture);
    SDL_DestroyTexture(tile13_texture);
    SDL_DestroyTexture(tile14_texture);
    SDL_DestroyTexture(tile15_texture);
    SDL_DestroyTexture(tile16_texture);
    SDL_DestroyTexture(tile17_texture);
    SDL_DestroyTexture(tile18_texture);
    SDL_DestroyTexture(tile19_texture);
    SDL_DestroyTexture(tile20_texture);
    SDL_DestroyTexture(tile21_texture);
    SDL_DestroyTexture(tile22_texture);
    SDL_DestroyTexture(tile23_texture);
    SDL_DestroyTexture(tile24_texture);
    SDL_DestroyTexture(tile25_texture);
    SDL_DestroyTexture(background_texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_CloseFont(font);
    TTF_Quit();
    SDL_Quit();
}
