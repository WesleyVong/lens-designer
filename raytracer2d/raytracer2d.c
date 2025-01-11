#include "raytracer2d.h"
#define _USE_MATH_DEFINES
#include "math.h"
#include "ior.h"
#include "stdlib.h"

void raytrace_ray(Raytracer2d * rt, Ray2d * r);

Raytracer2d * raytracer2d_init(long max_bounces, long max_objects){
    Raytracer2d * rt = (Raytracer2d *)malloc(sizeof(Raytracer2d));
    rt->max_bounces = max_bounces;
    rt->max_objects = max_objects;
    rt->num_objects = 0;
    rt->objects = malloc(max_objects * sizeof(Surface2d *));
    return rt;
}

void raytracer2d_free(Raytracer2d * rt){
    free(rt);
}

// Derived from the GLSL document page 153
vec2 refract(vec2 i, vec2 n, double eta1, double eta2) {
    // Assuming n1 and n2 are absolute, i.e relative to vacuum, get ratio of medium to get relative IOR
    double eta = eta1 / eta2;  
    double k = 1 - eta * eta * (1 - vec2_dot(n, i) * vec2_dot(n, i));
    if (k < 0){
        return (vec2){0,0};
    }
    vec2 eta_i = vec2_mul(i, eta);
    vec2 eta_n = vec2_mul(n, eta * vec2_dot(n, i) + sqrt(k));
    vec2 result = (vec2){eta_i.x - eta_n.x, eta_i.y - eta_n.y};
    return result;
}

void raytracer2d_add_object(Raytracer2d * rt, Object2d * obj){
    if (rt->num_objects >= rt->max_objects){
        return;
    }
    rt->objects[rt->num_objects] = obj;
    rt->num_objects++;
}

void raytrace(Raytracer2d * rt){
    for (long r = 0; r < rt->num_rays; r++){
        raytrace_ray(rt, rt->rays[r]);
    }
}

void raytrace_ray(Raytracer2d * rt, Ray2d * r){
    // Since this is a sequential raytracer, trace through all raytracer objects in order
    Ray2d * current_ray = r;
    for (long o = 0; o < rt->num_objects; o++){
        Object2d * obj = rt->objects[o];
        for (long s = 0; s < obj->num_surfaces; s++){
            Surface2d * surface = obj->surfaces[s];
            Intersection2d i = surface->get_intersection(surface, current_ray);
            *current_ray->intersection = i;
            if (i.type == INTERSECT_NONE){
                continue;
            }
            current_ray->refracted_rays_num = 1;
            double ior1;
            double ior2;
            // We are assuming the IOR of air is 1
            // Ray entering object
            if (s == 0){
                ior1 = 1;
                ior2 = sellmeier(&obj->materials[s]->ior, current_ray->wavelength);
            }
            // Ray exiting object
            else if (s == obj->num_surfaces - 1){
                ior1 = sellmeier(&obj->materials[s-1]->ior, current_ray->wavelength);
                ior2 = 1;
            }
            // Ray inside object
            else {
                ior1 = sellmeier(&obj->materials[s-1]->ior, current_ray->wavelength);
                ior2 = sellmeier(&obj->materials[s]->ior, current_ray->wavelength);
            }

            vec2 refraction_ray_origin = i.point;
            vec2 refraction_ray_direction = refract(current_ray->direction, i.normal, ior1, ior2);

            current_ray->refracted_rays = ray2d_init(RAY_REFRACTED, refraction_ray_origin, refraction_ray_direction, 
                                                    current_ray->wavelength, current_ray->intensity, NULL);
            current_ray = current_ray->refracted_rays;
        }
    }
}

