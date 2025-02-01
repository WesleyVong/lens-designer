#ifndef RAYTRACER2D_H
#define RAYTRACER2D_H

#include "ray2d.h"
#include "surface.h"

typedef struct {
    long max_objects;

    long num_rays;
    Ray2d ** rays;

    long num_surfaces;
    Surface2d ** surfaces;

    long num_materials;
    Material ** materials;
} Raytracer2d;

Raytracer2d * raytracer2d_init();
void raytracer2d_free(Raytracer2d * rt);
void raytracer2d_add_surface(Raytracer2d * rt, Surface2d * surf);
void raytracer2d_add_material(Raytracer2d * rt, Material * mat);
void raytrace(Raytracer2d * rt);

#endif