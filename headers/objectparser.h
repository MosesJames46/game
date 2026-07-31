//Take a fille string with object information and parse
#ifndef OBJECTPARSER_H
#define OBJECTPARSER_H
#define OUTPUT_LINE(string, start, end) printf("%.*s", end - start, string + start)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#define PRINT(format, array, size) for(int i = 0; i < size; i++)\
                                        printf(format, array[i]);

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

#endif