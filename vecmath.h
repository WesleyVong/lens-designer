#include "stdint.h"

#ifndef VECMATH_H
#define VECMATH_H

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