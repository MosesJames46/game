#include "../headers/game_string.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

const decimal_char decimal = '.';
const negative_char negative = '-';
const char_offset_number char_offset = 48;

int game_stoi(game_stringy string){
    int size = string.size;
    if (size == 0) return 0;

    int sign = 1;
    if (string.string[0] == negative) sign = -1;
    
    int index = 0;
    int result = 0;
    if (sign < index) index++;

    while (index < size){
        if (string.string[index] == decimal) break;
        result *= 10;
        int integer_value = string.string[index] - char_offset;
        result += integer_value;
        index++;
    }

    result *= sign;

    return result;
}

float game_stof(game_stringy string){
    int size = string.size;
    int decimal_location = -1;
    float digit_place = -1;
    int index = 0;
    //initialize i;
    if (string.string[index] == '-') index++;

    //Search for all digits before decimal.
    while (string.string[index] != '.' && index < size){
        if (digit_place == -1){
            digit_place = 1;
        }else{
            digit_place *= 10;
        }
        index++;
    }

    if (string.string[index] == '.') decimal_location = index;
    if (decimal_location == 0 || (string.string[0] == '-' && decimal_location == 1)) digit_place = .1f;

    float result = 0;
    int i = 0;
    if (string.string[0] == '-') i++;
    for(;i < size; i++){
        if (i == decimal_location) continue;
        result += ((int)(string.string[i]) - 48) * digit_place;
        digit_place /= 10;
    }
    if (string.string[0] == '-') result *= -1.0f;
    
    return result;
}

game_stringy game_create_stringy(char* string){
    game_stringy gm_stringy;
    gm_stringy.string = string;
    gm_stringy.size = strlen(string);
    return gm_stringy;
}

game_stringy game_create_stringy_be(char* begin, char* end){
    int length = end - begin;
    return (game_stringy){length, begin};
}


void game_print_stringy(game_stringy gm_stringy){
    //https://stackoverflow.com/questions/3767284/using-printf-with-a-non-null-terminated-string
    //The stack overflow link directs to how to use format to specify size of string to print.
    printf("%.*s", gm_stringy.size, gm_stringy.string);

}

game_stringy game_copy_stringc(char* string, int begin, int end){
    game_stringy gm_string;
    gm_string.size = end - begin;

    char* new_string = malloc(sizeof(char) * gm_string.size);
    for (int i = 0; i < gm_string.size; i++){
        new_string[i] = string[begin++];
    }
    gm_string.string = new_string;
    new_string = NULL;
    return gm_string;
}

game_stringy game_copy_string(game_stringy string, int begin, int end){
    if (end > string.size || begin > string.size) return (game_stringy){0, ""};
    return game_copy_stringc(string.string, begin, end);
}

void game_free_string(game_stringy* string){
    if (string->size > 0){
        string->size = 0;
        free(string->string);
    }
    
}

int find_next_space(game_stringy contents, int index){
    if (index > contents.size) return index;
    index++;
    while(contents.string[index] != ' ' && contents.string[index] != '\n' && index < contents.size){
        //printf("Contesnts size: %d\n", contents.size);
        index++;
    } 
    return index;
}

int find_next_non_whitespace(game_stringy contents, int index){
    //First, finds next whitespace. Then iterates until a nonwhitespace is founds.
    //printf("Current index: %d\n", index);
    index++;
    while(contents.string[index] == ' ' || contents.string[index] == '\n' && index < contents.size){
        index++;
    } 
    return index;
}

bool game_string_cmp(game_stringy find, game_stringy src){
    if (find.size == src.size){
        for(int i = 0; i < find.size; i++) if (find.string[i] != src.string[i])  return false;
        return true;
    }
    return false;
}

bool game_string_find(game_stringy search, game_stringy find){

    //Finds the string specified by the 'find' parameter. Does to by ignoring whitespaces.
    int skip = find.size;
    for(int i = 0; i < search.size && i + skip < search.size; i++){
        game_stringy temp = game_create_stringy_be(&search.string[i], &search.string[i + skip]);
        if (game_string_cmp(temp, find)) return true;
        find_next_non_whitespace(search, i);
    }
    return false;
}