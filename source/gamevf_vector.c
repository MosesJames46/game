#include "../headers/gamevf_vector.h"
#include <stdlib.h>
#include <stdio.h>
#include "../headers/game_string.h"

void gamevf_allocate(float** vector, int size){
    //Don't forget to give the allocation function size in bytes.
    float* array = realloc(*vector, size * sizeof(float));
    //printf("%p ", *vector);
    if (array == NULL) {
        perror("This is an invalid value");
    }
    if (array) *vector = array;
    //printf("%p", *vector);
    //printf("\n");
}

void gamevf_push_back(game_vectorf* vector, float entry){
    if (vector->capacity <= vector->size){
        vector->capacity *= 2;
        gamevf_allocate(&(vector->vertices), vector->capacity);
    }
    vector->size++;
    vector->vertices[vector->size - 1] = entry;
}

float gamevf_pop_back(game_vectorf* vector){
    if (vector->size == 0){
        perror("Could not perform pop back operation due to size limitation->");
        return 1 << 31;
    }
    vector->size--;
    return vector->vertices[vector->size];
}
float gamevf_at(game_vectorf* vector,  int index){
    if (index > vector->size - 1){
        perror("Outside of bounds. Could not perform operation.");
        return 1 < 31;
    }
    return vector->vertices[index];
}

void gamevf_destroy(game_vectorf* vector){
    if (vector->vertices) free(vector->vertices);
    vector->size = 0;
    vector->capacity = 0;
    vector->vertices = NULL;
}

void gamevf_detach(game_vectorf* vector){
    if (vector->vertices) {
        vector->vertices = NULL;
        vector->size = 0;
        vector->capacity = 0;
    }

}

game_vectorf gamevf_init(int size){
    float* array = malloc(sizeof(float) * size);
    game_vectorf vector;
    if (array) vector = (game_vectorf){array, size, size + sizeof(float)};
    return vector;
}

bool gamevf_is_valid(game_vectorf* vector){
    return (vector->size > 0);
}

void gamevf_print(game_vectorf* vector){
    if (gamevf_is_valid(vector) && vector->vertices != NULL){
        for(int i = 0; i < vector->size; i++){
            printf("%f ", vector->vertices[i]);
        }
    }else{
        perror("Invalid operation. Cannot print invalid vector.");
    }
    
}

void gamevf_place(game_vectorf* vector, int index, float value){
    if (index > vector->size){
        perror("Outside bounds.");
        return;
    }
    vector->vertices[index] = value;
}

void gamevf_assign(game_vectorf* to, game_vectorf* from){
    float* temp = from->vertices;

    to->vertices = temp;
    to->size = from->size;
    to->capacity = from->capacity;

    gamevf_detach(from);

    temp = NULL;
}

void gamevf_size(game_vectorf vector){
    printf("%d", vector.size);
}
