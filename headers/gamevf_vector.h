#ifndef GAME_VECTORF_H
#define GAME_VECTORF_H
#include <stdlib.h>
#include <stdbool.h>

typedef struct game_vectorf{
    float* vertices;
    int size;
    int capacity;
}game_vectorf;

void gamevf_push_back(game_vectorf* vector, float entry);
void gamevf_allocate(float** array, int size);
float gamevf_pop_back(game_vectorf* vector);
float gamevf_at(game_vectorf* vector, int index);

void gamevf_destroy(game_vectorf* vector);
void gamevf_detach(game_vectorf* vector);
void gamevf_assign(game_vectorf* to, game_vectorf* from);

game_vectorf gamevf_init(int size);
bool gamevf_is_valid(game_vectorf* vector);
void gamevf_print(game_vectorf* vector);
void gamevf_place(game_vectorf* vector, int index, float value);
void gamevf_size(game_vectorf vector);

#endif