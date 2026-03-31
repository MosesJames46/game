#include <stdio.h>
#include <stdbool.h>
#ifndef MODEL_H
#define MODEL_H

typedef struct Model{
    float* points;
    float* colors;
    FILE* file_location;
}Model;

Model* model_read_file(const char* file_location);
int model_get_file_size(Model* model);
void model_file_read(const char* file, Model* model);
void model_close_file(Model* model);
bool model_open_file(const char* file, Model* model);

//First goal is to read in vertices.
//Face values are represented as f: v1/vt/vn
//You can omit the texture coordinate using v1//vn
typedef float vertex[3];
typedef int face[3];

#endif