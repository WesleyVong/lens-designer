#ifndef SURFACE_H
#define SURFACE_H

#include "vecmath.h"
#include "intersection.h"
#include "ray2d.h"

typedef struct {
    vec2 (*get_point)(void * surface, double t);    // The shape of the object should be defined for 0 <= t <= 1
    Intersection2d (*get_intersection)(void * surface, Ray2d * r);
} Surface2d;

typedef struct {
    Surface2d surface;

    vec2 origin;
    vec2 direction;
    double length;
} Line;

typedef struct {
    Surface2d surface;

    vec2 origin;
    double radius;
    double start_angle;
    double end_angle;
} Arc;


Line * line_init(vec2 origin, vec2 direction, double length);
void line_free(Line * w);
vec2 line_shape(void * surface, double t);
Intersection2d line_intersection(void * obj, Ray2d * r);

Arc * arc_init(vec2 origin, double radius, double start_angle, double end_angle);
void arc_free(Arc * w);
vec2 arc_shape(void * surface, double t);
Intersection2d arc_intersection(void * obj, Ray2d * r);


#endif