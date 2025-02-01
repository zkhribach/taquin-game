#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <SDL.h>
#include <SDL_ttf.h>

#define WINDOW_WIDTH 600
#define WINDOW_HEIGHT 600
#ifdef _WIN32
#define FONT_PATH "C:\\Windows\\Fonts\\arial.ttf"
#else
#define FONT_PATH "/usr/share/fonts/TTF/Hack-Regular.ttf"
#endif

typedef struct {
    int i;
    int j;
} Index;

int random_(int n);
void tableau_vide(int chaine[100][100], int n);
void tableau_rand(int chaine[100][100], int n);
void swap(int g[][100], int empty_value, int n);
int check_win(int grid[][100], int n);
Index detect_index(int x, int t[][100], int n);
int est_solvable(int grid[100][100], int n);
int position_case_vide(int grid[100][100], int n);
int compter_inversions(int grid[100][100], int n);
void setup(int n, int g[100][100], int y[100][100]);
void render_update(int g[100][100], int n, Uint32 start_time, Uint32 *elapsed_time, SDL_Renderer *renderer, TTF_Font *font, int game_won);
void render_grid(int g[100][100], int n, SDL_Renderer *renderer, TTF_Font *font);
int initialize_window(void);
void process_input(SDL_Event *event, int g[100][100], int n);
void destroy_window(void);
int game_loop(int n, int g[100][100], SDL_Renderer *renderer, TTF_Font *font, int *menu_state);
void handle_event(SDL_Event *event, int g[100][100], int n, int *game_won);
void update_timer(Uint32 start_time, Uint32 *elapsed_time, int game_won);
void render_menu(SDL_Renderer *renderer, TTF_Font *font, int selected_option);
void process_menu_input(SDL_Event *event, int *selected_option, int *menu_state);
void process_start_input(SDL_Event *event, int *selected_option, int *menu_state);
void render_start(SDL_Renderer *renderer, TTF_Font *font, int selected_option);

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
TTF_Font* font = NULL;
SDL_Texture* tile_texture = NULL;
SDL_Texture* background_texture = NULL;
Uint32 start_time;
Uint32 elapsed_time = 0;
int game_won = 0;
int selected_option = 0;

enum GameState {
    START,
    MENU,
    GAME,
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
        }else if (game_state == MENU) {
            process_menu_input(&event, &selected_option, &game_state);
            render_menu(renderer, font, selected_option);
        } else if (game_state == GAME) {
            n = 3 + selected_option;
            setup(n, g, y);
            game_state = game_loop(n, g, renderer, font, &game_state);
        } else if (game_state == QUIT){
            break;
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

        handle_event(&event, g, n, &game_won);
        update_timer(start_time, &elapsed_time, game_won);

        render_update(g, n, start_time, &elapsed_time, renderer, font, game_won);

        SDL_Delay(1000 / 60);
    }
    return GAME;
}

void handle_event(SDL_Event *event, int g[100][100], int n, int *game_won) {
    process_input(event, g, n);

    if (check_win(g, n)) {
        *game_won = 1;
    }
}

void update_timer(Uint32 start_time, Uint32 *elapsed_time, int game_won) {
    if (!game_won) {
        *elapsed_time = (SDL_GetTicks() - start_time) / 1000;
    }
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
    SDL_RenderClear(renderer);

    SDL_RenderCopy(renderer, background_texture, NULL, NULL);

    render_grid(g, n, renderer, font);

    char time_text[32];
    sprintf(time_text, "Time: %d s", *elapsed_time);

    SDL_Color white = {0, 100, 255, 255};
    SDL_Surface* surface = TTF_RenderText_Solid(font, time_text, white);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_Rect dst = {WINDOW_WIDTH - surface->w - 10, 20, surface->w, surface->h};
    SDL_RenderCopy(renderer, texture, NULL, &dst);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);

    SDL_RenderPresent(renderer);
}

void render_grid(int g[100][100], int n, SDL_Renderer *renderer, TTF_Font *font) {
    int tile_width = ( WINDOW_WIDTH - 100 ) / n;
    int tile_height = WINDOW_HEIGHT / n;
    int margin = 10;

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (g[i][j] != 0) {
                SDL_Rect tile = {j * tile_width, i * tile_height, tile_width, tile_height};

                SDL_Rect inner_tile = {tile.x + margin, tile.y + margin, tile_width - 2 * margin, tile_height - 2 * margin};
                SDL_RenderCopy(renderer, tile_texture, NULL, &inner_tile);

                char number_text[2];
                sprintf(number_text, "%d", g[i][j]);
                SDL_Color text_color = {255, 0, 0, 255};
                SDL_Surface* surface = TTF_RenderText_Solid(font, number_text, text_color);
                SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
                SDL_Rect text_rect = {tile.x + tile_width / 2 - surface->w / 2, tile.y + tile_height / 2 - surface->h / 2, surface->w, surface->h};
                SDL_RenderCopy(renderer, texture, NULL, &text_rect);
                SDL_FreeSurface(surface);
                SDL_DestroyTexture(texture);
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

    SDL_Surface* background_surface = SDL_LoadBMP("wood.bmp");
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
    SDL_Surface* tile_surface = SDL_LoadBMP("tile.bmp");
    if (!tile_surface) {
        printf("Failed to load tile image: %s\n", SDL_GetError());
        return -1;
    }
    tile_texture = SDL_CreateTextureFromSurface(renderer, tile_surface);
    SDL_FreeSurface(tile_surface);
    if (!tile_texture) {
        printf("Failed to create texture from tile image: %s\n", SDL_GetError());
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
                *selected_option = (*selected_option ) % 2;
                break;
            case SDLK_UP:
                *selected_option = (*selected_option ) % 2;
                break;
            case SDLK_RETURN:
                *menu_state = MENU;
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
            default:
                break;
        }
    }
}

void render_start(SDL_Renderer *renderer, TTF_Font *font, int selected_option) {
    SDL_Color white = {255, 255, 255, 255};
    SDL_Color yellow = {255, 255, 0, 255};

    const char* options[] = { "Start Game" };
    SDL_Color colors[] = { white, white, white };
    colors[selected_option] = yellow;

    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, background_texture, NULL, NULL);
    const char* title = "Welcome To The Taquin Game";
    SDL_Surface* title_surface = TTF_RenderText_Solid(font, title, white);
    SDL_Texture* title_texture = SDL_CreateTextureFromSurface(renderer, title_surface);
    SDL_Rect title_rect = { WINDOW_WIDTH / 2 - title_surface->w / 2, WINDOW_HEIGHT / 6, title_surface->w, title_surface->h };
    SDL_RenderCopy(renderer, title_texture, NULL, &title_rect);
    SDL_FreeSurface(title_surface);
    SDL_DestroyTexture(title_texture);

    for (int i = 0; i < 1; i++) {
        SDL_Surface* surface = TTF_RenderText_Solid(font, options[i], colors[i]);
        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_Rect dst = { WINDOW_WIDTH / 2 - surface->w / 2, WINDOW_HEIGHT / 3 + i * 50, surface->w, surface->h };
        SDL_RenderCopy(renderer, texture, NULL, &dst);
        SDL_FreeSurface(surface);
        SDL_DestroyTexture(texture);
    }
    const char* credit = "Credit : Ziyad Khribach";
    SDL_Surface* credit_surface = TTF_RenderText_Solid(font, credit, white);
    SDL_Texture* credit_texture = SDL_CreateTextureFromSurface(renderer, credit_surface);
    SDL_Rect credit_rect = { WINDOW_WIDTH / 2 - credit_surface->w / 2, 4 * WINDOW_HEIGHT / 6, credit_surface->w, credit_surface->h };
    SDL_RenderCopy(renderer, credit_texture, NULL, &credit_rect);
    SDL_FreeSurface(credit_surface);
    SDL_DestroyTexture(credit_texture);

    SDL_RenderPresent(renderer);
}

void render_menu(SDL_Renderer *renderer, TTF_Font *font, int selected_option) {
    SDL_Color white = {255, 255, 255, 255};
    SDL_Color yellow = {255, 255, 0, 255};

    const char* options[] = { "3x3", "4x4", "5x5" };
    SDL_Color colors[] = { white, white, white };
    colors[selected_option] = yellow;

    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, background_texture, NULL, NULL);
    const char* title = "Select difficulty :";
    SDL_Surface* title_surface = TTF_RenderText_Solid(font, title, white);
    SDL_Texture* title_texture = SDL_CreateTextureFromSurface(renderer, title_surface);
    SDL_Rect title_rect = { WINDOW_WIDTH / 2 - title_surface->w / 2, WINDOW_HEIGHT / 6, title_surface->w, title_surface->h };
    SDL_RenderCopy(renderer, title_texture, NULL, &title_rect);
    SDL_FreeSurface(title_surface);
    SDL_DestroyTexture(title_texture);

    for (int i = 0; i < 3; i++) {
        SDL_Surface* surface = TTF_RenderText_Solid(font, options[i], colors[i]);
        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_Rect dst = { WINDOW_WIDTH / 2 - surface->w / 2, WINDOW_HEIGHT / 3 + i * 50, surface->w, surface->h };
        SDL_RenderCopy(renderer, texture, NULL, &dst);
        SDL_FreeSurface(surface);
        SDL_DestroyTexture(texture);
    }
    /*const char* credit = "Credit : Ziyad Khribach ";
    SDL_Surface* credit_surface = TTF_RenderText_Solid(font, credit, white);
    SDL_Texture* credit_texture = SDL_CreateTextureFromSurface(renderer, credit_surface);
    SDL_Rect credit_rect = { WINDOW_WIDTH / 2 - credit_surface->w / 2, 4 * WINDOW_HEIGHT / 6, credit_surface->w, credit_surface->h };
    SDL_RenderCopy(renderer, credit_texture, NULL, &credit_rect);
    SDL_FreeSurface(credit_surface);
    SDL_DestroyTexture(credit_texture);*/

    SDL_RenderPresent(renderer);
}

void destroy_window(void) {
    SDL_DestroyTexture(tile_texture);
    SDL_DestroyTexture(background_texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_CloseFont(font);
    TTF_Quit();
    SDL_Quit();
}
