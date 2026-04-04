#include "../headers/model.h"
#include "../headers/object.h"
#include <stdlib.h>
#include <string.h>

Model* model_read_file(const char* file){
    Model* model = malloc(sizeof(model));
    if (model_open_file(file, model)){
        model_file_read(file, model);
    }
    model_close_file(model);
    return model;
}

int model_get_file_size(Model* model){
    if (model->file_location == NULL) return -1;

    //How to find the size of a file.
    // https://stackoverflow.com/questions/238603/how-can-i-get-a-files-size-in-c

    //This seeks the EOF value. Once that value is found, it is assigned to file size.
    //However, before seeking, you need to make sure the file was even open succesfully. 
    int file_size;
    
    fseek(model->file_location, 0L, SEEK_END);
    
    file_size = ftell(model->file_location);

    //We have to rewind the file for some reason too. 
    rewind(model->file_location);
    return file_size;
}

void model_file_read(const char* file, Model* model){
    //How to read the contents of a file.
    //https://stackoverflow.com/questions/174531/how-to-read-the-content-of-a-file-to-a-string-in-c
    int file_size = model_get_file_size(model);
    char* contents;
    contents = malloc(sizeof(char) * file_size);

    //The link explains a bit about how fread works.
    //https://www.reddit.com/r/cs50/comments/gkry18/how_does_fread_function_work/
    /*
         fread reads exactly as many bytes as you tell it to (unless there are fewer bytes than that remaining in the file.)

        The middle two parameters of fread are the size of the object you want to read and the number of 
        objects of that size you want to read. For example fread(buffer, 256, 1, file) will read one 256-byte object 
        from the file, while fread(buffer, 4, 8, file) will read 8 4-byte objects from the file. 
    */
   int f_result = 0;
    if (contents){
        f_result = fread(contents, 1, file_size, model->file_location);
    }
    bool sucess = (f_result == file_size);
    game_stringy content_string = game_create_stringy(contents);

    int i = 0;
    //printf("%s", contents);
    //printf("size of file: %d\n", k.size);
    //printf("The size of the result: %d\n", f_result);

    //Make sure the file is closed and then set the NULL.
    fclose(model->file_location);
    //printf("%s", contents);
    model_read_obj(model, content_string);
    model->file_location = NULL;
}

bool model_open_file(const char* file, Model* model){
    model->file_location = fopen(file, "r");
    
    if (model->file_location == NULL){
        perror("Error: File could not be opened.\n");
        fclose(model->file_location);
        return false;
    }
    return true;
}

void model_close_file(Model* model){
    if (model->file_location != NULL) fclose(model->file_location);
    model->file_location = NULL;
}

void model_read_obj(Model* model, game_stringy contents){
    Face_Descriptor faces;
    Vertex_Descriptor vertices;
    object_extract_data(&contents, &faces, &vertices);
}
