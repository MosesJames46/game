#include "../headers/object.h"
#define ADDRESS_LOCATION(A) printf("%p\n", (void*)A)
#include <limits.h>
#include <float.h>

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


bool is_vertex(char* object, int p){
    return *(object + p) == 'v' && *(object + p + 1) == ' ';
}

bool is_face(char* object, int p){
    return *(object + p) == 'f' && *(object + p + 1) == ' ';
}

float* collect_vertex_data_from_buffer(char* buffer, int* start, int* end, int* size){
    int buffer_pointer = 0;

    bool newline = true;
    bool done = false;

    *size = 0;

    while(buffer[buffer_pointer] != '\0' && !done){
        if (newline){
            if(is_vertex(buffer, buffer_pointer)){
                 done = true;
                 *start = buffer_pointer;
                 break;
            }
            newline = false;
        }
        if (buffer[buffer_pointer] == '\n') newline = true;
        buffer_pointer++;
    }
    
    if (done){
        *start = buffer_pointer;
    }else{
        return NULL;
    }

    done = false;

    while(buffer[buffer_pointer] != '\0' && !done){
        
        if(newline){
            if(is_vertex(buffer, buffer_pointer)){
                (*size)++;
                newline = false;
            }else{
                done = true;
            }  
        }
        if (buffer[buffer_pointer] == '\n') newline = true;
        buffer_pointer++;
    }

    (*size) *= 3;
    float* vertex_array = malloc(sizeof(float) * *size);
    char* vertex = buffer + *start;
    int i = 0;
    while((vertex = strchr(++vertex, ' ')) != NULL && i < *size){
        vertex_array[i] = strtof(vertex, NULL);
        i++;
    }
    return vertex_array;
}

unsigned int* collect_face_data_from_buffer(char* buffer, int* start, int* end, int* size){
    //Finds the first f and moves the start pointer to it's location.
    //Counts each newline until null or until no more faces.
    //Takes the total count of newline chars and multiplies by 3 to get size.
    //Obtains the face unsigned int and places it into allocaed float array.
    int index = *start;
    *size = 0;

    while(buffer[index] != '\0' && !is_face(buffer, index)){
        index++;
    }

    if (buffer[index] == 'f'){
         *start = index;
    }else{
        return NULL;
    }

    char* face = strchr(buffer + index, ' ');
    char* sf_result;
    unsigned int ui_face = strtoul(face, &sf_result, 10);

    int slash_count = 0;
    
    if (*sf_result == '/') slash_count++;
    if (slash_count) ui_face = strtoul(++sf_result, &sf_result, 10);
    if (*face == '/') slash_count++;

    bool newline = true;
    bool done = false;

    while (buffer[index] != '\0' && !done){
        if (newline && is_face(buffer, index)){
            newline = false;
            (*size)++;
        }else if (newline && !is_face(buffer, index)){
            *end = index;
            done = true;
        }

        if (buffer[index] == '\n') newline = true;
        index++;
    }
    
    (*size) *= 3; 
    unsigned int* ui_face_array = malloc(sizeof(unsigned int) * (*size));

    int i = 0;
    face = buffer+ *start;
    while((face = strchr(++face, ' ')) != NULL && i < *size){
        ui_face = strtoul(face, NULL, 10);
        ui_face_array[i] = ui_face;
        i++;
    }

    return ui_face_array;
}

float* get_vertex_buffer(unsigned int* faces, int size, float* vertex_array){
    //Takes in an index array and returns all the connected vertex array
    //The size of the vertex buffer is going to be a multiple of 3 higher.
    //If there is 1 face, then there are 3 unsigned int that make some triangle.
    //There should then be 9 vertices that correspond to this face. 
    //Therefore the size of the vertex buffer is vb_size * 3
    float* vertex_buffer = malloc(sizeof(float) * size * 3);
    int vertex_buffer_size = size * 3;
    
    int i, v_i;
    for (i = v_i = 0; i < size; i += 3){
        unsigned int f1 = faces[i] - 1;
        unsigned int f2 = faces[i + 1] - 1;
        unsigned int f3 = faces[i + 2] - 1;

        int first = f1 * 3;
        int second = f2 * 3;
        int third = f3 * 3;
        
        int j = 0;
        while(j < 3){
            vertex_buffer[j + v_i] = vertex_array[first + j];
            j++;
        }
        v_i += 3;
        j = 0;
        while(j < 3){
            vertex_buffer[j + v_i] = vertex_array[second + j];
            j++;
        }
        v_i += 3;
        j = 0;
        while(j < 3){
            vertex_buffer[j + v_i] = vertex_array[third + j];
            j++;
        }
        v_i += 3;
    }

    return vertex_buffer;
}

struct Object* init_object(const char* location, const char* readmode){
    struct Object* object = malloc(sizeof(struct Object));
    object->buffer = read_in_file(location, readmode);
    //printf("%s\n", object->buffer);
    int start , end;
    start = end = 0;
    object->vertices = collect_vertex_data_from_buffer(object->buffer, &start, &end, &object->v_size);
    //PRINT("%f ", object->vertices, object->v_size);
    object->faces = collect_face_data_from_buffer(object->buffer, &start, &end, &object->f_size);
    //PRINT("%u ", object->faces, object->f_size);
    object->vb_size = object->f_size * 3;
    object->vertex_buffer = get_vertex_buffer(object->faces, object->f_size, object->vertices);
    //PRINT("%f ", object->vertex_buffer, object->f_size);
    //normalize_vertices(&object);
    return object;
}

void normalize_vertices(struct Object** object){
    //For the x and y values, find the min and max components and normalize
    //The expression to normalize is ((x - min) * 2) / distance) - 1;
    //This is a modified interpolation. We get the percentage of the value
    //from %0 to %200 or 0 - 2. By subtracting 1, we normalize the range from -1 to 1.
    float max_x = FLT_MIN;
    float min_x = FLT_MAX;

    float max_y = FLT_MIN;
    float min_y = FLT_MAX;

    float min_z = FLT_MAX;
    float max_z = FLT_MIN;

    int size = (*object)->v_size;
    for (int i = 0; i < size; i+=3){
        float x = (*object)->vertices[i];
        float y = (*object)->vertices[i + 1];
        float z = (*object)->vertices[i + 2];
        if (x > max_x) max_x = x;
        if (x < min_x) min_x = x;

        if (y > max_y) max_y = y;
        if (y < min_y) min_y = y;

        if (z > max_z) max_z = z;
        if (z < min_z) min_z = z;
    }

    float x_mag = max_x - min_x;
    float y_mag = max_y - min_y;
    float z_mag = max_z - min_z;
    for (int i = 0; i < size; i+=3){
        float x = (*object)->vertices[i];
        float y = (*object)->vertices[i + 1];
        float z = (*object)->vertices[i + 2];
        float norm_x = (((x - min_x) * 2.f) / x_mag) - 1.f;
        (*object)->vertices[i] = norm_x;
        float norm_y = (((y - min_y) * 2.f) / y_mag) - 1.f;
        (*object)->vertices[i + 1] = norm_y;
        float norm_z = (((z - min_z) * 2.f) / z_mag) - 1.f;
        (*object)->vertices[i + 2] = norm_z;
    }
}