#include "vecmath.h"
#include "ray.h"

#ifndef RAYTRACER_H
#define RAYTRACER_H

typedef enum {
    EMIT_SUN,
    EMIT_POINT
} EmitterType;

typedef struct {
    vec3 origin;
    double radius;
} sphere;

typedef struct {
    EmitterType type;
    void * emit;
    material * mat;
} emitter;

typedef struct {
    vec3 direction;
} sun;

typedef struct {
    long width;
    long height;

    long max_bounces;

    long rays_per_pixel;
    long num_rays;
    ray * rays;

    long num_objects;
    object * objects;

    long num_emitters;
    emitter * emitters;
} raytracer;

void generate_rays(raytracer * rt, vec3 eye, vec3 forward, vec3 up);

void raytrace_image(raytracer * rt, unsigned char * image);

void raytrace_free(raytracer * rt);

#endif