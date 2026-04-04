#include "../headers/object.h"


void object_extract_vertices(game_stringy* substring, game_vectorf* vector){
    /*
        Obtain every float within the vertex substring.

        Update the start position for each substring by obtaining the next space. 
    */
    int start = 1;
    while (start < substring->size){
        float ans = object_extract_vertex_data(*substring, start);
        //printf("%f ", ans);
        gamevf_push_back(vector, ans);
        start = find_next_space(*substring, start);
    }
    
}

float object_extract_vertex_data(game_stringy sub, int begin){
    /*
        Obtain wach individual float value but locating the char that begins the float and
        the char the ends the float. 

        Create a substring using the index of the the begin/end chars via game_copy_string.

        convert this string to a float. 
    */
    int space = find_next_non_whitespace(sub, begin);
    int next_space = find_next_space(sub, space);
    next_space--;
    //printf("Next space: %d\n", next_space);
    game_stringy temp_number = game_copy_string(sub, space, next_space);
    //game_print_stringy(temp_number);
    //begin = next_space;
    //printf("Begin value: %d, %d\n", space, next_space);
    float t = game_stof(temp_number);
    //printf(" T: %f ", t);
    return t;
}

void object_extract_data(game_stringy* contents, Face_Descriptor* faces, Vertex_Descriptor* vertices){
    /*
        The object file loader's goal is to read the contents outputted by the object file
        and place values into an array for processing by the renderer. 

        Each vertex, vertex texture, and vertext normal is obtained. The faces are then obtained.

        The information obtained from the object file is then sent to the object struct. Afterwards, 
        we allow the model to hold a pointer the object information and send this data to the renderer. 
    */

    int newline = 0;
    int begin_sub = 0;
    game_stringy string;

    object_init_vertex_descriptor(vertices);

    game_stringy v = game_create_stringy("v ");
    game_stringy vt  = game_create_stringy("vt ");
    game_stringy vn  = game_create_stringy("vn ");

    object_init_face_descriptor(faces);
    game_stringy f = game_create_stringy("f ");

    int position = 0;
    while(position < contents->size){
        while (contents->string[newline] != '\n') newline++;
        string = game_copy_string(*contents, position, newline);
        bool is_vert = game_string_find(string, v);
        bool is_vt = game_string_find(string, vt);
        bool is_vn = game_string_find(string, vn);
        bool is_f = game_string_find(string, f);

        if(is_vert){
            object_extract_vertices(&string, &vertices->vertex);
        } else if (is_vt){
            object_extract_vertices(&string, &vertices->texture);
        }else if(is_vn){
            object_extract_vertices(&string, &vertices->normal);
        }else if (is_f){
            object_extract_subface(&string, faces);
        }

        position = ++newline;
        game_free_string(&string);
    }
}

void object_extract_subface(game_stringy* substring, Face_Descriptor* faces){
    /*
        Starting at the beginning of the face substring, iterate through each
        face index v/vt/vn.

        Update the index by obtaining the next white space char index and end when the index
        matches the size of the substring.

        Using the face descriptor, update respective arrays if and only if the face array was updated
        with a value. The face descriptor will either be 0 or the same size as other faces.
    */
    

    int begin = 1;
    int face_array[3] = {-1, -1, -1};
    while (begin < substring->size - 1){
        object_extract_face_index(substring, begin, face_array);
        begin = find_next_space(*substring, begin);
        gamevi_push_back(&faces->vertex_indices, face_array[0]);
        if (face_array[1] != -1) gamevi_push_back(&faces->texture_indices, face_array[1]);
        if (face_array[2] != -1) gamevi_push_back(&faces->normal_indices, face_array[2]);
        face_array[0] = -1; face_array[1] = -1; face_array[2] = -1;
    }
}

void object_extract_face_index(game_stringy* substring, int begin, int* index_array){
    /*
        Find the begin and ending of a face by locating the beginning/ending char.

        There will be 3 maximum substrings within each face substring.

        Each face index can have 3 different face indices corresponding to vertex, vertex texture, and vertex normal
        respectively in that order. 

        We should take as input a float array of size 3. depending on number of slashes, depends when we 
        return from our function. Each input array should by default have each index value be -1.
        Function will fill the array with values if and only if respective slash is found.
    */


   /*
        While obtaining the begin/end chars we obtain and slash index if applicable. If there is a slash,
        we mark its location within the face substring. There can be at most 2 but at least one. If
        there is only one slash, make sure it's identified which vertex attribute it belongs to(texture/normal).
   */
   int first_char = find_next_non_whitespace(*substring, begin);
   int second_char = find_next_space(*substring, first_char);
   game_stringy temp = game_copy_string(*substring, first_char, second_char);
   first_char = 0;
   second_char = temp.size - 1; 


    int first_slash = 0;
    int second_slash = 0;
    for (int i = 0; i < temp.size; i++){
        if (temp.string[i] == '/'){
            if (!first_slash){
                first_slash = i;
            }else{
                second_slash = i;
            }
        }
    }
    
    /*
        Becuase it is entrierly possible for there to be 3 different integers within a single face attribute,
        we have to obtain these integers based off their position between their slashes

        First, if a slash is detected and the next digit is not a newline and not a slash, we know it's a number or
        a negative sign. 
            - We can either iterate until we find a new slash or space.  i.e. create substring with copy if second
            slash or create substring with find next space index - 1. 
    */
   game_stringy vertex;
   game_stringy vertex_texture;
   game_stringy vertex_normal;

   int v, vt, vn;


    /*
        If the first slash is found, create the vertex subtring using the first char of the
        face substring and the first slash index.

        Otherwise the face only contains a vertex index.
    */
   
    if (first_slash){
        vertex = game_copy_string(temp, first_char, first_slash);
        v = game_stoi(vertex);
        index_array[0] = v;

    }else{
        int next_space = find_next_space(temp, 0);
        v = game_stoi(temp);
        index_array[0] = v;
        game_free_string(&temp);
        return;
    }
    
    
    /*
        For our vertex texture, determine if there is a second slash.

        If there is a second slash, we can use the index of the first slash and second slash to get a subtring 
        of the vertex texture.

        If there is no second slash, we can use the position of our first slash and the position of the last
        char of our face substring to create the vertex texture substring as this implies that there is only a
        vertex and vertex texture belonging to the face substring.
    */
   //printf("%d\n", second_slash - first_slash);
   //printf("%d %d %d\n", first_char, second_char, temp.size); return;

    if (first_slash){
        if (!second_slash){
            vertex_texture = game_copy_string(temp, first_slash + 1, temp.size);
            vt = game_stoi(vertex_texture);
            index_array[1] = vt;
            game_free_string(&temp);
            game_free_string(&vertex_texture);
            return;
        }else if (second_slash - first_slash > 2){
            vertex_texture = game_copy_string(temp, first_slash + 1, second_slash);
            vt = game_stoi(vertex_texture);
            game_free_string(&vertex_texture);
            index_array[1] = vt;
        }
    }
    
    /*
        For our vertex normal, a second slash must exist. If it does, ensure that the slash index
        does not exceed that size of the face substring. 
        If the second slash index is less than the size of the substring, then we know we have a vertex normal.

        Otherwise there is no veterx normal.
    */
   //return;
   //printf("%d %d %d ", first_slash, second_slash, temp.size); return;
    if (second_slash && second_slash + 1  < temp.size){
        vertex_normal = game_copy_string(temp, second_slash + 1, temp.size);
        vn = game_stoi(vertex_normal);
        game_free_string(&vertex_normal);
        index_array[2] = vn;
    }
    game_free_string(&temp);
}

void object_init_face_descriptor(Face_Descriptor* face_descriptor){
    face_descriptor->vertex_indices = gamevi_init(0);
    face_descriptor->texture_indices = gamevi_init(0);
    face_descriptor->normal_indices = gamevi_init(0);
}

void object_init_vertex_descriptor(Vertex_Descriptor* vertex_descriptor){
    vertex_descriptor->vertex = gamevf_init(0);
    vertex_descriptor->texture = gamevf_init(0);
    vertex_descriptor->normal = gamevf_init(0);
}