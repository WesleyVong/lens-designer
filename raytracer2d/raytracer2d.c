#include "raytracer2d.h"
#include "math.h"

Raytracer2d * raytracer2d_init(long max_bounces, long max_objects){
    Raytracer2d * rt = (Raytracer2d *)malloc(sizeof(Raytracer2d));
    rt->max_bounces = max_bounces;
    rt->max_objects = max_objects;
    rt->num_objects = 0;
    rt->objects = (Object2d *)malloc(max_objects * sizeof(Object2d));
    return rt;
}

void raytracer2d_free(Raytracer2d * rt){
    free(rt);
}

void raytracer2d_add_object(Raytracer2d * rt, Object2d * obj){
    rt->objects[rt->num_objects] = *obj;
    rt->num_objects++;
}

void raytrace(Raytracer2d * rt, Ray2d * r){
    // Since this is a sequential raytracer, trace through all raytracer objects in order
}

