#include "raytracer.h"
#include "math.h"

double lambert(vec3 normal, vec3 light_dir){
    double lambert = vec3_dot(normal, light_dir);
    if (lambert < 0){
        lambert = 0;
    }
    return lambert;
}

intersection ray_intersect_nearest(raytracer * rt, ray * r);
void raytrace(raytracer * rt, ray * r);
ray ray_shadow(raytracer * rt, ray * r, emitter * e);
ray ray_reflect(raytracer * rt, ray * r);
vec3 ray_to_color(ray * r);

vec3 get_diffuse(ray * r);
vec3 get_shine(ray * r);

vec3 linear_to_srgb(vec3 color){
    if (color.x <= 0.0031308){
        color.x *= 12.92;
    } else {
        color.x = 1.055 * pow(color.x, 1/(2.4)) - 0.055;
    }
    if (color.y <= 0.0031308){
        color.y *= 12.92;
    } else {
        color.y = 1.055 * pow(color.y, 1/(2.4)) - 0.055;
    }
    if (color.z <= 0.0031308){
        color.z *= 12.92;
    } else {
        color.z = 1.055 * pow(color.z, 1/(2.4)) - 0.055;
    }
    return color;
}

intersection ray_sphere(ray * r, object * o){
    intersection i;
    i.type = INTERSECT_NONE;
    i.point = (vec3){0, 0, 0};
    i.normal = (vec3){0, 0, 0};
    i.distance = 0;
    i.obj = o;

    if (o->type != OBJECT_SPHERE){
        return i;
    }

    sphere * s = o->geometry;

    double rsq = s->radius * s->radius;
    double rdirlen = vec3_length(r->direction); // Length of ray direction

    // Distance between sphere and ray origin
    vec3 dp = vec3_sub(s->origin, r->origin);

    // Determine whether the ray starts inside or outside the sphere
    if (vec3_length_squared(dp) < rsq){
        i.type = INTERSECT_INTERIOR;
    }

    // Closest approach of ray to sphere center
    double tc = vec3_dot(r->direction, dp) / rdirlen;

    // No intersection if closest approach is behind the ray
    if (i.type != INTERSECT_INTERIOR && tc < 0){
        return i;
    }

    double dsq = vec3_length_squared(vec3_sub(vec3_add(r->origin, vec3_mul(r->direction, tc)), s->origin));

    // No intersection if closest approach is outside the sphere
    if (i.type != INTERSECT_INTERIOR && dsq > rsq){
        return i;
    }

    double toffset = sqrt(rsq - dsq) / rdirlen;

    if (i.type == INTERSECT_INTERIOR){
        i.distance = tc + toffset;
    } else {
        i.type = INTERSECT_EXTERIOR;
        i.distance = tc - toffset;
    }

    i.point = vec3_add(r->origin, vec3_mul(r->direction, i.distance));
    i.normal = vec3_normalize(vec3_sub(i.point, s->origin));
    i.mat = o->mat;

    return i;
}

// Finds the intersection point of a ray and an object
intersection ray_intersect(ray * r, object * o){
    intersection i;
    i.type = INTERSECT_NONE;
    i.point = (vec3){0, 0, 0};
    i.normal = (vec3){0, 0, 0};
    i.distance = 0;
    i.obj = o;

    if (o->type == OBJECT_SPHERE){
        return ray_sphere(r, o);
    }

    return i;
}

void raytrace_image(raytracer * rt, unsigned char * image){
    for (long pixel = 0; pixel < rt->width * rt->height; pixel++){
        raytrace(rt, &rt->rays[pixel]);
        vec3 color = ray_to_color(&rt->rays[pixel]);
        double alpha = 1;
        if (color.x < 0 || color.y < 0 || color.z < 0){
            alpha = 0;
        }
        color.x = (color.x > 1) ? 1 : color.x;
        color.y = (color.y > 1) ? 1 : color.y;
        color.z = (color.z > 1) ? 1 : color.z;

        color = linear_to_srgb(color);

        color = vec3_mul(color, 255);
        image[pixel * 4] = (unsigned char)color.x;
        image[pixel * 4 + 1] = (unsigned char)color.y;
        image[pixel * 4 + 2] = (unsigned char)color.z;
        image[pixel * 4 + 3] = (unsigned char)alpha * 255;
    }
}

void raytrace_free(raytracer * rt){
    if (rt->rays != NULL){
        for (long r = 0; r < rt->num_rays; r++){
            if (&rt->rays[r] != NULL){
                ray_free(&rt->rays[r]);
            }
        }
        free(rt->rays);
    }
    if (rt->objects != NULL){
        free(rt->objects);
    }
    if (rt->emitters != NULL){
        free(rt->emitters);
    }
}

void generate_rays(raytracer * rt, vec3 eye, vec3 forward, vec3 up){
    rt->num_rays = rt->width * rt->height * rt->rays_per_pixel;
    rt->rays = (ray*)malloc(rt->num_rays * sizeof(ray));
    vec3 right = vec3_normalize(vec3_cross(forward, up));
    up = vec3_normalize(vec3_cross(right, forward));
    double long_edge = (rt->width > rt->height) ? rt->width : rt->height;
    for (long i = 0; i < rt->num_rays; i++){
        long x = i % rt->width;
        long y = i / rt->width;
        double sx = (2 * x - rt->width) / long_edge;
        double sy = (rt->height - 2 * y) / long_edge;
        vec3 xr = vec3_mul(right, sx);
        vec3 yu = vec3_mul(up, sy);
        vec3 dir = vec3_normalize(vec3_add(forward, vec3_add(xr, yu)));
        ray_init(&rt->rays[i]);
        rt->rays[i].type = RAY_PRIMARY;
        rt->rays[i].origin = eye;
        rt->rays[i].direction = dir;
        rt->rays[i].bounces = 0;
    }
}

vec3 ray_to_color(ray * r){
    vec3 color = {-1, -1, -1};
    intersection i = *r->intersection;
    if (i.type == INTERSECT_NONE){
        return color;
    }

    // Get the diffuse color by mixing the shadows and the object color
    vec3 diffuse = get_diffuse(r);

    // Get the shine color by recursively looking through reflected rays and getting their colors
    vec3 shine = get_shine(r);
    
    diffuse = vec3_mul(diffuse, 1 - i.mat->shininess);
    shine = vec3_mul(shine, i.mat->shininess);
    color = vec3_add(diffuse, shine);
    return color;
}

vec3 get_diffuse(ray * r){
    vec3 diffuse = {0, 0, 0};
    if (r->intersection->type == INTERSECT_NONE){
        return diffuse;
    }
    for (long s = 0; s < r->shadow_rays_num; s++){
        if (r->shadow_rays[s].intersection->type == INTERSECT_NONE){
            double lambertian = lambert(r->intersection->normal, r->shadow_rays[s].intersection->normal);
            diffuse = vec3_add(diffuse, vec3_mul(vec3_vec3_mul(r->shadow_rays[s].intersection->mat->color, r->intersection->mat->color), lambertian));        
        }
    }
    return diffuse;
}

// Gets the shine of the object through recursion
// Go to the last bounce and work upwards, generating and combining the diffuse colors according to the material's shine
// Currently assume there is only one reflected ray per ray so no post order traversal is needed
vec3 get_shine(ray * r){
    vec3 diffuse = get_diffuse(r);
    if (r->reflected_rays_num == 0){
        return diffuse;
    }
    diffuse = vec3_mul(diffuse, 1 - r->intersection->mat->shininess);
    vec3 shine = get_shine(r->reflected_rays);
    shine = vec3_mul(shine, r->intersection->mat->shininess);
    shine = vec3_add(diffuse, shine);
    return shine;
}

void raytrace(raytracer * rt, ray * r){
    // Finds nearest intersection
    r->intersection = (intersection *)malloc(sizeof(intersection));
    *r->intersection = ray_intersect_nearest(rt, r);

    if (r->intersection->type == INTERSECT_NONE){ 
        return; // No intersection, no secondary rays
    }
    // Shadow ray generation/calculation
    r->shadow_rays_num = rt->num_emitters;
    r->shadow_rays = (ray *)malloc(r->shadow_rays_num * sizeof(ray));
    for (long e = 0; e < rt->num_emitters; e++){
        r->shadow_rays[e] = ray_shadow(rt, r, &rt->emitters[e]);
    }
    // Reflection ray generation/calculation
    r->reflected_rays_num = 0;
    if (r->bounces < rt->max_bounces){
        r->reflected_rays_num = 1;
        r->reflected_rays = (ray *)malloc(r->reflected_rays_num * sizeof(ray));
        r->reflected_rays[0] = ray_reflect(rt, r);
    }
    // Refraction ray generation/calculation
    // TODO:

    for (long s = 0; s < r->reflected_rays_num; s++){
        raytrace(rt, &r->reflected_rays[s]);
    }
}

intersection ray_intersect_nearest(raytracer * rt, ray * r){
    double nearest_distance = INFINITY;
    intersection nearest;
    nearest.type = INTERSECT_NONE;

    for (int idx = 0; idx < rt->num_objects; idx++){
        intersection i = ray_intersect(r, &rt->objects[idx]);
        if (i.type != INTERSECT_NONE && i.distance > 0){
            double intersect_distance = vec3_length(i.point);   // Assuming eye is at origin
            if (intersect_distance < nearest_distance){
                nearest = i;
                nearest_distance = intersect_distance;
            }
        }
    }
    return nearest;
}

ray ray_shadow(raytracer * rt, ray * r, emitter * e){
    ray shadow_ray;
    ray_init(&shadow_ray);
    shadow_ray.type = RAY_SHADOW;
    shadow_ray.intersection = (intersection *)malloc(sizeof(intersection));
    shadow_ray.bounces = r->bounces + 1;
    intersection_init(shadow_ray.intersection);
    if (r->intersection->type == INTERSECT_NONE){
        return shadow_ray;
    }
    if (e->type == EMIT_SUN){
        sun * s = (sun *)e->emit;
        shadow_ray.origin = r->intersection->point;
        shadow_ray.direction = s->direction;
        for (int o = 0; o < rt->num_objects; o++){
            // Remove self intersections
            if (&rt->objects[o] == r->intersection->obj){
                continue;
            }
            *shadow_ray.intersection = ray_intersect(&shadow_ray, &rt->objects[o]);
            if (shadow_ray.intersection->type != INTERSECT_NONE){
                return shadow_ray;
            }
        }
        shadow_ray.intersection->mat = e->mat;
        shadow_ray.intersection->normal = s->direction;
    }
    return shadow_ray;
}

double epsilon = 0.0001; // Offset to prevent self intersection
ray ray_reflect(raytracer * rt, ray * r){
    ray reflected_ray;
    ray_init(&reflected_ray);
    reflected_ray.type = RAY_REFLECTED;
    // Reflected ray direction as described in GLSL page 153
    reflected_ray.direction = vec3_sub(r->direction, vec3_mul(r->intersection->normal, 2 * vec3_dot(r->intersection->normal, r->direction)));
    reflected_ray.origin = vec3_add(r->intersection->point, vec3_mul(reflected_ray.direction, epsilon));
    reflected_ray.intersection = (intersection *)malloc(sizeof(intersection));
    reflected_ray.bounces = r->bounces + 1;
    intersection_init(reflected_ray.intersection);

    return reflected_ray;
}

