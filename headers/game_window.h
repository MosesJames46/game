#include <SDL2/SDL.h>
#include <stdbool.h>
#ifndef GAME_WINDOW_H
#define GAME_WINDOW_H

#define WINDOW_WIDTH 800;
#define WINDOW_HEIGHT 600;

typedef struct game_window{
    int width;
    int height;
    SDL_Window* game_window;
}game_window;

bool init_game_window(game_window* game_window);


#endif