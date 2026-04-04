#include <stdio.h>
#include <stdbool.h>
#include "game_string.h"
#include "gamevf_vector.h"
#include "gamevi_vector.h"
#ifndef MODEL_H
#define MODEL_H

typedef struct Model{
    float* vertices;
    float* textures;
    float* normals;

    int* indices;

    int* colors;
    FILE* file_location;
}Model;

Model* model_read_file(const char* file_location);
int model_get_file_size(Model* model);
void model_file_read(const char* file, Model* model);
void model_close_file(Model* model);
bool model_open_file(const char* file, Model* model);
void model_read_obj(Model* model, game_stringy contents);;

#endif