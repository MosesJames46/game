/*
    This header file is created to assist with converting strings to basic types such as
    int, float, double etc...
*/

typedef char negative_char;
extern const negative_char negative;

typedef char decimal_char;
extern const decimal_char decimal;

typedef int char_offset_number;
extern const char_offset_number char_offset;

int game_stoi(char* string);
float game_stof(char* string);