#ifndef GAME_VECTORI_H
#define GAME_VECTORI_H
#include <stdlib.h>
#include <stdbool.h>

typedef struct game_vectori{
    int* vertices;
    int size;
    int capacity;
}game_vectori;

void gamevi_push_back(game_vectori* vector, int entry);
void gamevi_allocate(int** array, int size);
int gamevi_pop_back(game_vectori* vector);
int gamevi_at(game_vectori* vector, int index);

void gamevi_destroy(game_vectori* vector);
void gamevi_detach(game_vectori* vector);
void gamevi_assign(game_vectori* to, game_vectori* from);

game_vectori gamevi_init(int size);
bool gamevi_is_valid(game_vectori* vector);
void gamevi_print(game_vectori* vector);
void gamevi_place(game_vectori* vector, int index, int value);
void gamevi_size(game_vectori vector);

#endif