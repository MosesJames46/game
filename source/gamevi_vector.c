#include "../headers/gamevi_vector.h"
#include <stdlib.h>
#include <stdio.h>
#include "../headers/game_string.h"

void gamevi_allocate(int** vector, int size){
    //Don't forget to give the allocation function size in bytes.
    int* array = realloc(*vector, size * sizeof(int));
    //printf("%p ", *vector);
    if (array == NULL) {
        perror("This is an invalid value");
    }
    if (array) *vector = array;
    //printf("%p", *vector);
    //printf("\n");
}

void gamevi_push_back(game_vectori* vector, int entry){
    if (vector->capacity <= vector->size){
        vector->capacity *= 2;
        gamevi_allocate(&(vector->vertices), vector->capacity);
    }
    vector->size++;
    vector->vertices[vector->size - 1] = entry;
}

int gamevi_pop_back(game_vectori* vector){
    if (vector->size == 0){
        perror("Could not perform pop back operation due to size limitation->");
        return 1 << 31;
    }
    vector->size--;
    return vector->vertices[vector->size];
}
int gamevi_at(game_vectori* vector,  int index){
    if (index > vector->size - 1){
        perror("Outside of bounds. Could not perform operation.");
        return 1 < 31;
    }
    return vector->vertices[index];
}

void gamevi_destroy(game_vectori* vector){
    if (vector->vertices) free(vector->vertices);
    vector->size = 0;
    vector->capacity = 0;
    vector->vertices = NULL;
}

void gamevi_detach(game_vectori* vector){
    if (vector->vertices){
         vector->vertices = NULL;
         vector->size = 0;
         vector->capacity = 0;
    }
}

void gamevi_assign(game_vectori* to, game_vectori* from){
    int* temp = from->vertices;

    to->vertices = temp;
    to->size = from->size;
    to->capacity = from->capacity;

    gamevi_detach(from);

    temp = NULL;
}

game_vectori gamevi_init(int size){
    int* array = malloc(sizeof(int) * size);
    game_vectori vector;
    if (array) vector = (game_vectori){array, size, size + sizeof(int)};
    return vector;
}

bool gamevi_is_valid(game_vectori* vector){
    return (vector->size > 0);
}

void gamevi_print(game_vectori* vector){
    if (gamevi_is_valid(vector) && vector->vertices != NULL){
        for(int i = 0; i < vector->size; i++){
            printf("%d ", vector->vertices[i]);
        }
    }else{
        perror("Invalid operation. Cannot print invalid vector.");
    }
    
}

void gamevi_place(game_vectori* vector, int index, int value){
    if (index > vector->size){
        perror("Outside bounds.");
        return;
    }
    vector->vertices[index] = value;
}

void gamevi_size(game_vectori vector){
    printf("%d", vector.size);
}