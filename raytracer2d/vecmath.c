#include "vecmath.h"
#include "math.h"

// Vector 2D functions

vec2 vec2_add(vec2 a, vec2 b){
    vec2 result = {a.x + b.x, a.y + b.y};
    return result;
}

vec2 vec2_sub(vec2 a, vec2 b){
    vec2 result = {a.x - b.x, a.y - b.y};
    return result;
}

vec2 vec2_mul(vec2 a, double b){
    vec2 result = {a.x * b, a.y * b};
    return result;
}

vec2 vec2_div(vec2 a, double b){
    vec2 result = {a.x / b, a.y / b};
    return result;
}

vec2 vec2_neg(vec2 a){
    vec2 result = {-a.x, -a.y};
    return result;
}

vec2 vec2_vec2_mul(vec2 a, vec2 b){
    vec2 result = {a.x * b.x, a.y * b.y};
    return result;
}

double vec2_dot(vec2 a, vec2 b){
    return a.x * b.x + a.y * b.y;
}

double vec2_length(vec2 a){
    return sqrt(a.x * a.x + a.y * a.y);
}

double vec2_length_squared(vec2 a){
    return a.x * a.x + a.y * a.y;
}

vec2 vec2_normalize(vec2 a){
    return vec2_div(a, vec2_length(a));
}

double vec2_angle(vec2 direction, vec2 normal){
    // Reshuffled dot product equation
    double cos_theta = vec2_dot(direction, normal) / (vec2_length(direction) * vec2_length(normal));
    double angle = arccos(cos_theta);
    return angle;
}

// Vector 3D functions

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