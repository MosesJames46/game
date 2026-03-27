#include "../headers/game_data.h"

bool init_game(Game_Data* game_data){
    game_data->width = WINDOW_WIDTH;
    game_data->height = WINDOW_HEIGHT;
    if(SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("Failed to initialize the SDL2 library\n");
        return false;
    }
    game_data->window= SDL_CreateWindow("SDL2 Window",
                                          SDL_WINDOWPOS_CENTERED,
                                          SDL_WINDOWPOS_CENTERED,
                                          game_data->width, game_data->height,
                                          0);

    if(!game_data->window)
    {
        printf("Failed to create window\n");
        return false;
    }

    SDL_Surface *window_surface = SDL_GetWindowSurface((game_data->window));

    if(!window_surface)
    {
        printf("Failed to get the surface from the window\n");
        return false;
    }
    SDL_CreateWindowAndRenderer(game_data->width, game_data->height, 0, &game_data->window, &game_data->renderer);
    
    return true;
}