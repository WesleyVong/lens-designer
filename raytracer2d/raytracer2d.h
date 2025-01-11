#ifndef RAYTRACER2D_H
#define RAYTRACER2D_H

#include "ray2d.h"
#include "object.h"

typedef struct {
    long max_bounces;
    long max_objects;

    long num_rays;
    Ray2d ** rays;

    long num_objects;
    Object2d ** objects;
} Raytracer2d;

Raytracer2d * raytracer2d_init(long max_bounces, long max_objects);
void raytracer2d_free(Raytracer2d * rt);
void raytracer2d_add_object(Raytracer2d * rt, Object2d * obj);
void raytrace(Raytracer2d * rt);

#endif