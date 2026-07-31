#include "../headers/objectparser.h"

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