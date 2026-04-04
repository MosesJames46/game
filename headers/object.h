/*
    This header file is created for the purpose of reading object files.
    It parses an object file and holds the contents.
*/
#include "gamevf_vector.h"
#include "game_string.h"
#include "gamevi_vector.h"

#ifndef OBJECT_H
#define OBJECT_H

typedef struct Face_Descriptor{
    game_vectori vertex_indices;
    game_vectori texture_indices;
    game_vectori normal_indices;
}Face_Descriptor;

typedef struct Vertex_Descriptor{
    game_vectorf vertex;
    game_vectorf texture;
    game_vectorf normal;
}Vertex_Descriptor;

float object_extract_vertex_data(game_stringy sub, int begin);
void object_extract_vertices(game_stringy* substring, game_vectorf* vector);

void object_extract_face_index(game_stringy* substring, int begin, int* index_array);
void object_extract_subface(game_stringy* substring, Face_Descriptor* faces);
void object_extract_data(game_stringy* contents, Face_Descriptor* faces, Vertex_Descriptor* vertices);

void object_fill_model(float* vertices, int indices);
void object_init_face_descriptor(Face_Descriptor* face_descriptor);
void object_init_vertex_descriptor(Vertex_Descriptor* vertex_descriptor);

#endif