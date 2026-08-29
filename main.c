#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>
#include <stdio.h>
#include <stdbool.h>
#include "headers/draw.h"
#include "headers/object.h"
#include "headers/Camera.h"
#include <time.h>
#include "headers/Matrix4.h"

//"/usr/local//opt/libomp/include"

/*
    Some Books to eventually get into: 
    Computer Graphics: Principles and Practice — Foley, van Dam, Feiner, Hughes

*/

/*
    GCC compiling SDL

    https://www.reddit.com/r/cpp_questions/comments/1g2vsj7/how_are_you_supposed_to_use_a_library_like_sdl2/
*/


int main(int argc, char* argv []){
    bool initialized = init_game();
    SDL_Event e;
    const float delta = 1.f/60.f;

    float dt = 0;
    float z_axis = 0;
    float current_time = SDL_GetTicks() / 1000;

    //struct Object* object = init_object("game/diablo3_pose.obj", "rb");
    struct Object* object = init_object("game/obj files/teapot.obj", "rb");

    bool quit = (initialized) ? false : true;
    float time_now = SDL_GetTicks() / 1000;
    float time_before = time_now;
    
    while (!quit){
        //Clear screen
        clock_t before = clock();
        SDL_SetRenderDrawColor(game_data.renderer, 0, 0, 0, 255);
        SDL_RenderClear(game_data.renderer);
        //Reset z-buffer every frame.

        for (int i =0; i < game_data.width; i++){
            for (int j = 0; j < game_data.height; j++){
                game_data.z_buffer[i][j] = 0;
            }
        }

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
        
        draw_object(object, 150);
        SDL_RenderPresent(game_data.renderer);
        clock_t after = clock();
        double time_spent = (double)(after - before) / CLOCKS_PER_SEC;
        //printf("Time elapsed: %f\n", time_spent);
    }
    
    return 0;
}