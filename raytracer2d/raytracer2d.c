#include "raytracer2d.h"
#define _USE_MATH_DEFINES
#include "math.h"
#include "ior.h"

Raytracer2d * raytracer2d_init(long max_bounces, long max_objects){
    Raytracer2d * rt = (Raytracer2d *)malloc(sizeof(Raytracer2d));
    rt->max_bounces = max_bounces;
    rt->max_objects = max_objects;
    rt->num_objects = 0;
    rt->objects = (Object2d **)malloc(max_objects * sizeof(Object2d *));
    return rt;
}

void raytracer2d_free(Raytracer2d * rt){
    free(rt);
}

// Derived from the GLSL document page 153
vec2 refract(vec2 i, vec2 n, double eta1, double eta2) {
    // Assuming n1 and n2 are absolute, i.e relative to vacuum, get ratio of medium to get relative IOR
    double eta = eta2 / eta1;  
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

void raytrace(Raytracer2d * rt, Ray2d * r){
    // Since this is a sequential raytracer, trace through all raytracer objects in order
    Ray2d * current_ray = r;
    for (long o = 0; o < rt->num_objects; o++){
        Object2d * obj = rt->objects[o];
        Intersection2d i = obj->get_intersection(obj, current_ray);
        *current_ray->intersection = i;
        current_ray->refracted_rays_num = 1;
        vec2 new_origin = i.point;
        // We are assuming the IOR of air is 1
        double ior1 = (r->mat == NULL) ? 1 : sellmeier(&r->mat->ior, r->wavelength);   // IOR of material ray is coming from
        double ior2 = (i.type == INTERSECT_INTERIOR) ? 1 : sellmeier(&obj->mat->ior, r->wavelength);   // IOR of material ray is going into

        vec2 refraction_direction = refract(r->direction, i.normal, ior1, ior2);

        current_ray->refracted_rays = ray2d_init(RAY_REFRACTED, i.point, refraction_direction, 
                                                current_ray->wavelength, current_ray->intensity, obj->mat);
        current_ray = current_ray->refracted_rays;
    }
}

