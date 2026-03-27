#include <stdio.h>
#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdbool.h>
#include "headers/draw.h"
#include "headers/game_utility.h"
#include "headers/vector2.h"


int main(int argc, char* argv []){
    Game_Data game_data;
    bool initialized = init_game(&game_data);

    
    vec2 u = {0, 0};
    vec2 v = {100, 0};
    vec2 w = {100, 100};
    //draw_line(u, v, window);

    SDL_Event e;
    
    bool quit = (initialized) ? false : true;
    while (!quit){
        SDL_SetRenderDrawColor(game_data.renderer, 0, 0, 0, 255);
        SDL_RenderClear(game_data.renderer);
        while (SDL_PollEvent(&e)){
            if (e.type == SDL_QUIT){
                quit = true;
            }
            if (e.type == SDL_KEYDOWN){
                quit = true;
            }
            if (e.type == SDL_MOUSEBUTTONDOWN){
                quit = true;
            }
        }
        draw_line(u, v, game_data);
        draw_line(v, w, game_data);
        draw_line(w, u, game_data);
        SDL_RenderPresent(game_data.renderer);
    }

    
    return 0;
}