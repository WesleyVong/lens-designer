#ifndef OBJECT_H
#define OBJECT_H

#include "vecmath.h"
#include "ray2d.h"
#include "color.h"

typedef struct {
    long id;
    char * name;
    Material * mat;
    vec2 (*shape)(void * obj, double t);
    Intersection2d (*wall_intersection)(void * obj, Ray2d * r);

    vec2 origin;
    vec2 direction;
    double length;
} Wall;

Wall * wall_init(long id, char * name, Material * mat, vec2 origin, vec2 direction, double length);
void wall_free(Wall * w);
vec2 wall_shape(void * obj, double t);
Intersection2d wall_intersection(void * obj, Ray2d * r);


#endif