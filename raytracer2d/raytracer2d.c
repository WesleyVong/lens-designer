#include "raytracer2d.h"
#include "math.h"
#include "ior.h"

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
    if (rt->num_objects >= rt->max_objects){
        return;
    }
    rt->objects[rt->num_objects] = *obj;
    rt->num_objects++;
}

void raytrace(Raytracer2d * rt, Ray2d * r){
    // Since this is a sequential raytracer, trace through all raytracer objects in order
    Ray2d * current_ray = r;
    for (long o = 0; o < rt->num_objects; o++){
        Object2d * obj = &rt->objects[o];
        Intersection2d i = obj->get_intersection(obj, current_ray);
        *current_ray->intersection = i;
        current_ray->refracted_rays_num = 1;
        vec2 new_origin = i.point;
        double normal_angle = vec2_angle(i.normal, (vec2){1, 0});
        double ray_angle_neg = vec2_angle(current_ray->direction, (vec2){1, 0});
        double intersect_angle = normal_angle - ray_angle_neg;
        double refraction_angle = get_refraction_angle(intersect_angle, 1, 1);
        vec2 refraction_direction = i.normal;
        double ior;
        current_ray->refracted_rays = ray2d_init(RAY_REFRACTED, i.point, refraction_direction, 
                                                current_ray->wavelength, current_ray->intensity, ior);
        current_ray = current_ray->refracted_rays;
    }
}

