#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>
#include <stdio.h>
#include <stdbool.h>
#include "headers/draw.h"
#include "headers/game_utility.h"
#include "headers/vector3.h"
#include "headers/Matrix3x3.h"



int main(int argc, char* argv []){
    bool initialized = init_game();
    //draw_line(u, v, window);
    //p1 = mat3x3_rotate((vec3){0, 180, 0}, p1);
    vec3 axis = {0, 0, 0};
    vec3 p1 = {-100, 0, 0};
    vec3 p2 = {100, 0.f, 0};
    vec3 p3 = {100, 100, 0};

    SDL_Event e;
    const float delta = 1.f/60.f;

    float dt = 0;
    float z_axis = 0;
    float current_time = SDL_GetTicks() / 1000;
    
    bool quit = (initialized) ? false : true;
    while (!quit){
        dt += 1 * delta;
        //printf("%f\n", z_axis);

        //Clear screen
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

        //draw_point(p1);
        draw_triangle((Triangle){p1, p2, p3});
        //vec3 bp = triangle_barycentric((Triangle){p1, p2, p3}, (vec3){50, 50, 0});
        //printf("hi\n");
        //printf("Time: %f\n", dt);
        //printf(VECTOR3_OUTPUT"\n", bp.x, bp.y, bp.z);
        SDL_RenderPresent(game_data.renderer);
    }
    printf("Total time: %f\n", dt);
    
    return 0;
}