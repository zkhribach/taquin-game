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






