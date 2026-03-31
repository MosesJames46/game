#include "../headers/game_string.h"
#include <string.h>
#include <stdio.h>

const decimal_char decimal = '.';
const negative_char negative = '-';
const char_offset_number char_offset = 48;

int game_stoi(char* string){
    int size = strlen(string);
    if (size == 0) return 0;

    int sign = 1;
    if (string[0] == negative) sign = -1;
    
    int index = 0;
    int result = 0;
    if (sign < index) index++;

    while (index < size){
        if (string[index] == decimal) break;
        result *= 10;
        int integer_value = string[index] - char_offset;
        result += integer_value;
        index++;
    }

    result *= sign;

    return result;
}

float game_stof(char* string){
    int size = strlen(string);

    int decimal_index = 0;
    while (decimal_index < size && string[decimal_index] != decimal) decimal_index++;

    char decimal_string[size - decimal_index];
    strcpy(decimal_string, &string[decimal_index + 1]);
   
    int before_decimal = game_stoi(string);
    int after_decimal = game_stoi(decimal_string);
    
    float result = after_decimal;
    while (result > 1) result /= 10;

    

    int sign = (before_decimal & 0x80000000) >> 31;
    if (sign) result *= sign;
    result += (float)before_decimal;
    return result;

}