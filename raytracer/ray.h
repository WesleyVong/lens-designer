#include "vecmath.h"

#ifndef RAY_H
#define RAY_H

typedef enum {
    RAY_PRIMARY,
    RAY_SHADOW,
    RAY_REFLECTED,
    RAY_REFRACTED
} RayType;

typedef enum {
    INTERSECT_NONE,
    INTERSECT_INTERIOR, // Ray intersects with inside face of object
    INTERSECT_EXTERIOR, // Ray intersects with outside face of object
} IntersectType;

typedef enum {
    OBJECT_SPHERE,
    OBJECT_PARAMETRIC
} ObjectType;

// TODO: Support wavelength based colors. Currently split RGB to three channels
typedef struct {
    vec3 color;
    double shininess;
} material;

typedef struct {
    ObjectType type;
    long id;
    void * geometry;
    material * mat;
} object;

typedef struct {
    IntersectType type;
    vec3 point;
    vec3 normal;
    double distance;   // The distance along the ray to the intersection point
    material * mat;       // Material that the ray intersects with
    object * obj;
} intersection;

typedef struct ray ray;

struct ray {
    RayType type;
    vec3 origin;
    vec3 direction;
    long bounces;
    double wavelength;
    double intensity;
    intersection * intersection;
    long shadow_rays_num;
    ray * shadow_rays;
    long reflected_rays_num;
    ray * reflected_rays;
    long refracted_rays_num;
    ray * refracted_rays;
};

void ray_init(ray * r);
void intersection_init(intersection * i);
void ray_free(ray * r);

#endif