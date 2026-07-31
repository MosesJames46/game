
#ifndef OBJECT_H
#define OBJECT_H
#include <stdio.h>
#include <stdlib.h>
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

#endif