#include "../headers/object.h"
#define ADDRESS_LOCATION(A) printf("%p\n", (void*)A)

int open_file(FILE** file, const char* file_location, const char* read_mode){
    //Attempts to create a file object. Returns zero on success.
    *file = fopen(file_location, read_mode);
    if (file) return 0;
    return 1;
}

int get_buffer_size(FILE* stream){
    fseek(stream, 0, SEEK_END);
    int size = ftell(stream);
    fseek(stream, 0, SEEK_SET);
    return size;
}

int allocate_buffer(char** buffer, int buffer_size){
    *buffer = malloc(sizeof(char) * buffer_size);
    if (*buffer) return 0;
    return -1;
}

int file_to_buffer(FILE* stream, char** buffer, int buffer_size){
    //https://en.cppreference.com/c/io/fread
    size_t status = fread(*buffer, sizeof(*buffer), buffer_size, stream);
    if (status < (size_t)buffer_size) return -1;
    return 0;
}


char* read_in_file(const char* file_location, const char* read_mode){
    FILE* file;
    char* buffer;
    int size = 0;
    int status;

    status = open_file(&file, file_location, read_mode);
    size = get_buffer_size(file);
    allocate_buffer(&buffer, size);
    file_to_buffer(file, &buffer, size);
    if (file) fclose(file);
    return buffer;
}
