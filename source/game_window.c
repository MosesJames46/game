#include "../headers/game_window.h"

bool init_game_window(game_window* g_window){
    g_window->width = WINDOW_WIDTH;
    g_window->height = WINDOW_HEIGHT;
    if(SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("Failed to initialize the SDL2 library\n");
        return false;
    }
    g_window->game_window = SDL_CreateWindow("SDL2 Window",
                                          SDL_WINDOWPOS_CENTERED,
                                          SDL_WINDOWPOS_CENTERED,
                                          g_window->width, g_window->height,
                                          0);

    if(!g_window->game_window)
    {
        printf("Failed to create window\n");
        return false;
    }

    SDL_Surface *window_surface = SDL_GetWindowSurface((g_window->game_window));

    if(!window_surface)
    {
        printf("Failed to get the surface from the window\n");
        return false;
    }
    
    return true;
}