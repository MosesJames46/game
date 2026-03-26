#include "../headers/game_math.h"

float gm_roundf(float x){
    int temp = x;
    x -= temp;
    if (x * 2.0f >= 1) return (float)temp + 1.0f;
    return (float)temp;
}