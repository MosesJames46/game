#include "../headers/game_math.h"

const float PI = 3.14159f;

float gm_roundf(float x){
    
    int temp = x;
    //0x80000000 Where the sign bit is located. By shifting 31 to the right, we make the int value 1.
    //Also, only returns a negative if temp as negative bit at x80000000
    int sign = temp & 0x80000000;
    sign = (sign >> 31);

    //If sign isn't negative it will return zero after shift.
    if (sign == 0) sign = 1;

    
    //Wanted to make sure temp was positive before rounding. Sign will change if and only if temp is negative.
    temp *= sign;
    x *= sign;
    x -= temp;

    //Turn back to negative only if sign is negative
    if (x * 2.0f >= 1) return (float) (temp + 1.0f) * sign;
    return (float)temp * sign;
}

float gm_minf(float x, float y){
    if (x < y) return x;
    return y;
}

float gm_maxf(float x, float y){
    if (x > y) return x;
    return y;
}