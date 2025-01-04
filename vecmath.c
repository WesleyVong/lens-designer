#include "vecmath.h"
#include "math.h"

vec3 vec3_add(vec3 a, vec3 b){
    vec3 result = {a.x + b.x, a.y + b.y, a.z + b.z};
    return result;
}

vec3 vec3_sub(vec3 a, vec3 b){
    vec3 result = {a.x - b.x, a.y - b.y, a.z - b.z};
    return result;
}

vec3 vec3_mul(vec3 a, double b){
    vec3 result = {a.x * b, a.y * b, a.z * b};
    return result;
}

vec3 vec3_div(vec3 a, double b){
    vec3 result = {a.x / b, a.y / b, a.z / b};
    return result;
}

vec3 vec3_neg(vec3 a){
    vec3 result = {-a.x, -a.y, -a.z};
    return result;
}

vec3 vec3_vec3_mul(vec3 a, vec3 b){
    vec3 result = {a.x * b.x, a.y * b.y, a.z * b.z};
    return result;
}

double vec3_dot(vec3 a, vec3 b){
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

vec3 vec3_cross(vec3 a, vec3 b){
    vec3 result = {a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x};
    return result;
}

double vec3_length(vec3 a){
    return sqrt(a.x * a.x + a.y * a.y + a.z * a.z);
}

double vec3_length_squared(vec3 a){
    return a.x * a.x + a.y * a.y + a.z * a.z;
}

vec3 vec3_normalize(vec3 a){
    return vec3_div(a, vec3_length(a));
}