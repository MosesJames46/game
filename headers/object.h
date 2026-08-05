
#ifndef OBJECT_H
#define OBJECT_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#define OUTPUT_LINE(string, start, end) printf("%.*s", end - start, string + start)
#define PRINT(format, array, size) \
            for(int i = 0; i < size; i++){\
            if (i % 3 == 0) printf("\n");\
            printf(format, array[i]);}

struct Object{
    char* buffer;
    float* vertices;
    int v_size;
    unsigned int* faces;
    int f_size;
    float* vertex_buffer;
    int vb_size;
};

/*
    Open the file to read.
    Obtain the size of the file.
    Allocate a buffer the fize of the file to read into. 
    Read the file into the allocated buffer.
    Return the pointer to the allocated pointer
*/

static int open_file(FILE** file, const char* file_location, const char* read_mode);
static int get_buffer_size(FILE* stream);
static int allocate_buffer(char** buffer, int buffer_size);
static int file_to_buffer(FILE* stream, char** buffer, int buffer_size);
char* read_in_file(const char* file_location, const char* read_mode);

/*
    1. Get newline
    2. Test if line has desired information
    3. Convert float strings to floats
        - Call the strtof on the line.
        https://en.cppreference.com/c/string/byte/strtof
    4. Place each float component into a buffer
    5. Return buffer.
*/

bool is_vertex(char* object, int p);
char* get_spaces(char* string);
float* collect_vertex_data_from_buffer(char* buffer, int* begin, int* end, int* size);
unsigned int* collect_face_data_from_buffer(char* buffer, int* start, int* end, int* size);
float* get_vertex_buffer(unsigned int* faces, int size, float* vertex_array);

struct Object* init_object(const char* location, const char* readmode);
void normalize_vertices(struct Object** object);

#endif