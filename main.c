#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>
#include <stdio.h>
#include <stdbool.h>
#include "headers/draw.h"
#include "headers/objectparser.h"
#include "headers/object.h"
#include "headers/Matrix3x3.h"

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

    int v_begin, v_end, v_size;
    v_begin = v_end = v_size = 0;
    char* object = read_in_file("teapot.obj", "rb");
    float* vertices = collect_vertex_data_from_buffer(object, &v_begin, &v_end, &v_size);

    int f_begin, f_end, f_size;
    f_begin = f_end = f_size = 0;
    unsigned int* faces = collect_face_data_from_buffer(object, &f_begin, &f_end, &f_size);

    float* vertex_buffer = get_vertex_buffer(faces, f_size, vertices);
    
    bool quit = (initialized) ? false : true;
    while (!quit){
        float time_now = SDL_GetTicks() / 1000;
        float time_passed = time_now - current_time;
        current_time = time_now;
        dt += delta;

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

        draw_object(faces, f_size, vertices, v_size);
        for (int i = 0; i < v_size; i+=3){
            vec3 r = mat3x3_rotate((vec3){0, dt, 0}, (vec3){vertices[i], vertices[i + 1], vertices[i + 2]});
            vertices[i] = r.x;
            vertices[i + 1] = r.y;
            vertices[i + 2] = r.z;
        }
        SDL_RenderPresent(game_data.renderer);
    }
    
    return 0;
}