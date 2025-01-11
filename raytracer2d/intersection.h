#ifndef INTERSECTION_H
#define INTERSECTION_H

#include "vecmath.h"

typedef enum {
    INTERSECT_NONE,
    INTERSECT_VALID,
    INTERSECT_INTERIOR, // Ray intersects with inside face of object
    INTERSECT_EXTERIOR, // Ray intersects with outside face of object
} Intersect2dType;

typedef struct {
    Intersect2dType type;
    vec2 point;
    vec2 normal;
    double distance;   // The distance along the ray to the intersection point
} Intersection2d;

#endif