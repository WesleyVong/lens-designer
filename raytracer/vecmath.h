#include "stdint.h"

#ifndef VECMATH_H
#define VECMATH_H

// 2D vector

typedef struct {
    double x, y;
} vec2;

vec2 vec2_add(vec2 a, vec2 b);
vec2 vec2_sub(vec2 a, vec2 b);
vec2 vec2_mul(vec2 a, double b);
vec2 vec2_div(vec2 a, double b);
vec2 vec2_neg(vec2 a);

vec2 vec2_vec2_mul(vec2 a, vec2 b);

double vec2_dot(vec2 a, vec2 b);

double vec2_length(vec2 a);
double vec2_length_squared(vec2 a);

vec2 vec2_normalize(vec2 a);

// 3D vector

typedef struct {
    double x, y, z;
} vec3;

vec3 vec3_add(vec3 a, vec3 b);
vec3 vec3_sub(vec3 a, vec3 b);
vec3 vec3_mul(vec3 a, double b);
vec3 vec3_div(vec3 a, double b);
vec3 vec3_neg(vec3 a);

vec3 vec3_vec3_mul(vec3 a, vec3 b);

double vec3_dot(vec3 a, vec3 b);
vec3 vec3_cross(vec3 a, vec3 b);

double vec3_length(vec3 a);
double vec3_length_squared(vec3 a);

vec3 vec3_normalize(vec3 a);

#endif