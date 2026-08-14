#include "../headers/game_data.h"


struct Game_Data game_data;

bool init_game(){
    game_data.width = WINDOW_WIDTH;
    game_data.height = WINDOW_HEIGHT;
    if(SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("Failed to initialize the SDL2 library\n");
        return false;
    }
    game_data.window= SDL_CreateWindow("SDL2 Window",
                                          SDL_WINDOWPOS_CENTERED,
                                          SDL_WINDOWPOS_CENTERED,
                                          game_data.width, game_data.height,
                                          0);

    if(!game_data.window)
    {
        printf("Failed to create window\n");
        return false;
    }

    SDL_Surface *window_surface = SDL_GetWindowSurface((game_data.window));

    if(!window_surface)
    {
        printf("Failed to get the surface from the window\n");
        return false;
    }
    SDL_CreateWindowAndRenderer(game_data.width, game_data.height, 0, &game_data.window, &game_data.renderer);
    
    game_data.z_buffer = malloc(sizeof(float*) * (game_data.width));
    for (int i = 0; i < game_data.width; i++){
        game_data.z_buffer[i] = malloc(sizeof(float) * game_data.height);
    }
    if (game_data.z_buffer){
        for(int i = 0; i < game_data.width; i++){
            for(int j = 0; j < game_data.height; j++){
                game_data.z_buffer[i][j] = 0;
            }
        }
    }

    return true;
}