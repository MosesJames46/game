#include <stdio.h>
#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdbool.h>
#include "headers/draw.h"
#include "headers/game_utility.h"
#include "headers/vector2.h"


int main(int argc, char* argv []){
    game_window window;
    bool initialized = init_game_window(&window);

    vec2 v = {250,250};
    vec2 u = {50, 8};
    //draw_line(u, v, window);

    SDL_Event e;
    SDL_Renderer* Renderer;
    SDL_CreateWindowAndRenderer(window.width, window.height, 0, &window.game_window, &Renderer);
    bool quit = (initialized) ? false : true;
    while (!quit){
        SDL_RenderClear(Renderer);
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
        SDL_SetRenderDrawColor(Renderer, 0, 0, 0, 0);
        SDL_RenderClear(Renderer);
        SDL_SetRenderDrawColor(Renderer, 255, 0, 0, 255);
        draw_line(u, v, window, Renderer);
        SDL_RenderPresent(Renderer);
    }

    
    return 0;
}