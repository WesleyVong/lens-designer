#ifndef RAY2D_H
#define RAY2D_H

#include "vecmath.h"
#include "ior.h"
#include "color.h"
#include "intersection.h"

typedef enum {
    RAY_PRIMARY,
    RAY_REFLECTED,
    RAY_REFRACTED
} Ray2dType;

typedef struct {
    Color diffuse;
    double shininess;
    double transparency;
    Sellmeier ior;  // Sellmeier coefficients for index of refraction calculation
} Material;

typedef struct Ray2d Ray2d;

struct Ray2d {
    Ray2dType type;
    vec2 origin;
    vec2 direction;
    long bounces;
    double wavelength;
    double intensity;
    Material * mat;    // IOR for current material it is going through

    Intersection2d * intersection;
    long reflected_rays_num;
    Ray2d * reflected_rays;
    long refracted_rays_num;
    Ray2d * refracted_rays;
};

Intersection2d * intersection2d_init(Intersect2dType type, vec2 point, vec2 normal, double distance);
void intersection2d_free(Intersection2d * i);
Ray2d * ray2d_init(Ray2dType type, vec2 origin, vec2 direction, double wavelength, double intensity, Material * mat);
void ray2d_free(Ray2d * r);

#endif