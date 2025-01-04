#include "ray.h"

void ray_init(ray * r){
    r->type = RAY_PRIMARY;
    r->origin = (vec3){0, 0, 0};
    r->direction = (vec3){0, 0, 0};
    r->bounces = 0;
    r->wavelength = 0;
    r->intensity = 0;
    r->intersection = NULL;
    r->shadow_rays_num = 0;
    r->shadow_rays = NULL;
    r->reflected_rays_num = 0;
    r->reflected_rays = NULL;
    r->refracted_rays_num = 0;
    r->refracted_rays = NULL;
}

void intersection_init(intersection * i){
    i->type = INTERSECT_NONE;
    i->point = (vec3){0, 0, 0};
    i->normal = (vec3){0, 0, 0};
    i->mat = NULL;
    i->distance = 0;
    i->obj = NULL;
}

void ray_free(ray * r){
    if (r->intersection != NULL){
        free(r->intersection);
    }
    if (r->shadow_rays != NULL){
        for (long s = 0; s < r->shadow_rays_num; s++){
            ray_free(&r->shadow_rays[s]);
        }
        free(r->shadow_rays);
    }
    if (r->reflected_rays != NULL){
        for (long rr = 0; rr < r->reflected_rays_num; rr++){
            ray_free(&r->reflected_rays[rr]);
        }
        free(r->reflected_rays);
    }
    if (r->refracted_rays != NULL){
        for (long rr = 0; rr < r->refracted_rays_num; rr++){
            ray_free(&r->refracted_rays[rr]);
        }
        free(r->refracted_rays);
    }
}