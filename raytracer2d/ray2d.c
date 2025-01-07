#include "ray2d.h"

Ray2d * ray2d_init(Ray2dType type, vec2 origin, vec2 direction, double wavelength, double intensity, double ior){
    Ray2d * r = (Ray2d *)malloc(sizeof(Ray2d));
    r->type = type;
    r->origin = origin;
    r->direction = vec2_normalize(direction);
    r->wavelength = wavelength;
    r->intensity = intensity;
    r->ior = ior;
    r->intersection = intersection2d_init(INTERSECT_NONE, (vec2){0, 0}, (vec2){0, 0}, 0);
    r->reflected_rays_num = 0;
    r->reflected_rays = NULL;
    r->refracted_rays_num = 0;
    r->refracted_rays = NULL;
    return r;
}

Intersection2d * intersection2d_init(Intersect2dType type, vec2 point, vec2 normal, double distance){
    Intersection2d * i = (Intersection2d *)malloc(sizeof(Intersection2d));
    i->type = type;
    i->point = point;
    i->normal = normal;
    i->distance = distance;
    return i;
}

void ray2d_free(Ray2d * r){
    intersection2d_free(r->intersection);
    free(r);
}

void intersection2d_free(Intersection2d * i){
    free(i);
}