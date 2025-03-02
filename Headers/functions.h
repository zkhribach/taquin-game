#ifndef function
#define function


typedef struct {
    int i;
    int j;
} Index;

typedef struct {
    char nom[100];
    int temps;
} Score;

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
int handle_event(SDL_Event *event, int g[100][100], int n, int *game_won);
void update_timer(Uint32 start_time, Uint32 *elapsed_time);
void render_menu(SDL_Renderer *renderer, TTF_Font *font, int selected_option);
void process_menu_input(SDL_Event *event, int *selected_option, int *menu_state);
void process_start_input(SDL_Event *event, int *selected_option, int *menu_state);
void render_start(SDL_Renderer *renderer, TTF_Font *font, int selected_option);
void render_credit(SDL_Renderer *renderer, TTF_Font *font, int selected_option);
void process_credit_input(SDL_Event *event, int *selected_option, int *menu_state);
void sauvegarder_score(const char* nom, Uint32 *elapsed_time);
char* afficher_meilleur_score();
void render_win(SDL_Renderer *renderer, TTF_Font *font, int selected_option);

#endif