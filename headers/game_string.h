#include <stdbool.h>


/*
    This header file is created to assist with converting strings to basic types such as
    int, float, double etc...
*/
#ifndef GAME_STRING_H
#define GAME_STRING_H
typedef char negative_char;
extern const negative_char negative;

typedef char decimal_char;
extern const decimal_char decimal;

typedef int char_offset_number;
extern const char_offset_number char_offset;

typedef struct game_stringy{
    unsigned int size;
    char* string;
}game_stringy;

int game_stoi(game_stringy string);
float game_stof(game_stringy string);

game_stringy game_create_stringy(char* string);

game_stringy game_copy_stringc(char* string, int begin, int end);
game_stringy game_copy_string(game_stringy string, int begin, int end);
void game_free_string(game_stringy* string);

void game_print_stringy(game_stringy gm_stringy);
int find_next_space(game_stringy contents, int index);
bool game_string_cmp(game_stringy find, game_stringy src);
bool game_string_find(game_stringy search, game_stringy find);
int find_next_non_whitespace(game_stringy contents, int index);
bool game_string_cmp(game_stringy find, game_stringy src);
#endif
