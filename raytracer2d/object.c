#include "object.h"

Wall * wall_init(long id, char * name, Material * mat, vec2 origin, vec2 direction, double length){
    Wall * w = (Wall *)malloc(sizeof(Wall));
    w->id = id;
    w->name = name;
    w->mat = mat;
    w->origin = origin;
    w->direction = vec2_normalize(direction);
    w->length = length;
    
    w->shape = wall_shape;
    w->wall_intersection = wall_intersection;
    return w;
}

void wall_free(Wall * w){
    free(w);
}

vec2 wall_shape(void * obj, double t){
    Wall * w = (Wall *)obj;
    return vec2_add(w->origin, vec2_mul(w->direction, w->length * t));
}

Intersection2d wall_intersection(void * obj, Ray2d * r){
    Wall * w = (Wall *)obj;
    Intersection2d i;
    i.type = INTERSECT_NONE;
    // Check if the ray and wall are parallel
    if (r->direction.x == w->direction.x && r->direction.y == w->direction.y){
        return i;
    }
    // Find intersection point of ray and wall
    double rt = (r->origin.y * w->direction.x + w->origin.x * w->direction.y
                - w->origin.y * w->direction.x - r->origin.x * w->direction.y) 
                / (r->direction.x * w->direction.y - r->direction.y * w->direction.x);
    if (rt < 0){
        return i;
    }
    double wt = (r->origin.x + r->direction.x * rt - w->origin.x) / w->direction.x;
    if (wt < 0 || wt > w->length){
        return i;
    }
    i.type = INTERSECT_EXTERIOR;
    i.point = vec2_add(r->origin, vec2_mul(r->direction, rt));
    // The normal is the perpendicular vector of the wall pointing towards the ray
    vec2 normal = vec2_normalize((vec2){w->direction.y, -w->direction.x});
    if (vec2_dot(normal, r->direction) < 0){
        normal = vec2_neg(normal);
    }
    i.normal = normal;
    i.distance = rt;
    return i;
}