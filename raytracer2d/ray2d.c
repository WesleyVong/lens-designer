#include "ray2d.h"

void ray2d_init(ray2d * r){
    r->type = RAY_PRIMARY;
    r->origin = (vec2){0, 0};
    r->direction = (vec2){0, 0};
    r->bounces = 0;
    r->wavelength = 0;
    r->intensity = 1;
    r->intersection = NULL;
    r->reflected_rays_num = 0;
    r->reflected_rays = NULL;
    r->refracted_rays_num = 0;
    r->refracted_rays = NULL;
}
void intersection2d_init(intersection2d * i){
    i->type = INTERSECT_NONE;
    i->point = (vec2){0, 0};
    i->normal = (vec2){0, 0};
    i->distance = 0;
    i->mat = NULL;
    i->obj = NULL;
}
void ray2d_free(ray2d * r){
    if (r->intersection != NULL){
        free(r->intersection);
    }
    if (r->reflected_rays != NULL){
        free(r->reflected_rays);
    }
    if (r->refracted_rays != NULL){
        free(r->refracted_rays);
    }
}