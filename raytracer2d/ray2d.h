#ifndef RAY2D_H
#define RAY2D_H

#include "vecmath.h"

typedef enum {
    RAY_PRIMARY,
    RAY_REFLECTED,
    RAY_REFRACTED
} Ray2dType;

typedef enum {
    INTERSECT_NONE,
    INTERSECT_INTERIOR, // Ray intersects with inside face of object
    INTERSECT_EXTERIOR, // Ray intersects with outside face of object
} Intersect2dType;

typedef enum {
    OBJECT_LINE,
    OBJECT_PARAMETRIC
} Object2dType;

// TODO: Support wavelength based colors. Currently split RGB to three channels
typedef struct {
    vec3 color;
    double shininess;
} material2d;

typedef struct {
    Object2dType type;
    long id;
    void * geometry;
    material2d * mat;
} object2d;

typedef struct {
    Intersect2dType type;
    vec2 point;
    vec2 normal;
    double distance;   // The distance along the ray to the intersection point
    material2d * mat;       // Material that the ray intersects with
    object2d * obj;
} intersection2d;

typedef struct ray2d ray2d;

struct ray2d {
    Ray2dType type;
    vec2 origin;
    vec2 direction;
    long bounces;
    double wavelength;
    double intensity;
    intersection2d * intersection;
    long reflected_rays_num;
    ray2d * reflected_rays;
    long refracted_rays_num;
    ray2d * refracted_rays;
};

void ray2d_init(ray2d * r);
void intersection2d_init(intersection2d * i);
void ray2d_free(ray2d * r);

#endif