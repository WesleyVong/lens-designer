#include "surface.h"
#include "stdlib.h"
#include "math.h"

Line * line_init(vec2 origin, vec2 direction, double length){
    Line * l = malloc(sizeof(Line));
    l->origin = origin;
    l->direction = vec2_normalize(direction);
    l->length = length;
    
    l->surface.get_point = line_shape;
    l->surface.get_intersection = line_intersection;
    return l;
}
void line_free(Line * l){
    free(l);
}
vec2 line_shape(void * surface, double t){
    Line * l = (Line *)surface;
    return vec2_add(l->origin, vec2_mul(l->direction, l->length * (t-0.5)));
}
Intersection2d line_intersection(void * obj, Ray2d * r){
    Line * l = (Line *)obj;
    Intersection2d i;
    i.type = INTERSECT_NONE;
    // Check that ray and line direction exists
    if (r->direction.x == 0 && r->direction.y == 0){
        return i;
    }
    if (l->direction.x == 0 && l->direction.y == 0){
        return i;
    }
    // Check if the ray and line are parallel
    if (r->direction.x == l->direction.x && r->direction.y == l->direction.y){
        return i;
    }
    double rt;  // Intersection time for ray
    double lt;  // Intersection time for line
    // Find intersection point of ray and line
    rt = (r->origin.y * l->direction.x + l->direction.y * l->origin.x - l->origin.y * l->direction.x - l->direction.y * r->origin.x)
            / (r->direction.x * l->direction.y - r->direction.y * l->direction.x);
    if (rt < 0){
        return i;
    }
    lt = (r->origin.x + r->direction.x * rt - l->origin.x) / l->direction.x;
    if (lt < -(l->length/2) || lt > (l->length/2)){
        return i;
    }
    i.type = INTERSECT_VALID;
    i.point = vec2_add(r->origin, vec2_mul(r->direction, rt));
    // The normal is the perpendicular vector of the line pointing towards the ray
    vec2 normal = vec2_normalize((vec2){l->direction.y, -l->direction.x});
    if (vec2_dot(normal, r->direction) > 0){
        normal = vec2_neg(normal);
    }
    i.normal = normal;
    i.distance = rt;
    return i;
}

Arc * arc_init(vec2 origin, double radius, double start_angle, double end_angle){
    Arc * a = malloc(sizeof(Arc));
    a->origin = origin;
    a->radius = radius;
    a->start_angle = start_angle;
    a->end_angle = end_angle;
    
    a->surface.get_point = arc_shape;
    a->surface.get_intersection = arc_intersection;
    return a;
}
void arc_free(Arc * a){
    free(a);
}
vec2 arc_shape(void * surface, double t){
    Arc * a = (Arc *)surface;
    double angle = a->start_angle + (a->end_angle - a->start_angle) * t;
    vec2 point = (vec2){a->radius * cos(angle), a->radius * sin(angle)};
    return vec2_add(a->origin, point);
}
Intersection2d arc_intersection(void * obj, Ray2d * r){
    Arc * a = (Arc *)obj;
    Intersection2d i;
    i.type = INTERSECT_NONE;
    // Check that ray exists
    if (r->direction.x == 0 && r->direction.y == 0){
        return i;
    }
    // Derived from the ray sphere intersection formula
    double rsq = a->radius * a->radius;
    // Distance between circle and ray origin
    vec2 dp = vec2_sub(a->origin, r->origin);

    // Determine whether the ray starts inside or outside the circle
    if (vec2_length_squared(dp) < rsq){
        i.type = INTERSECT_INTERIOR;
    }

    // Closest approach of ray to circle center
    double tc = vec2_dot(r->direction, dp) / vec2_length(r->direction);

    // No intersection if closest approach is behind the ray
    if (i.type != INTERSECT_INTERIOR && tc < 0){
        return i;
    }

    double dsq = vec2_length_squared(vec2_sub(vec2_add(r->origin, vec2_mul(r->direction, tc)), a->origin));

    // No intersection if closest approach is outside the circle
    if (i.type != INTERSECT_INTERIOR && dsq > rsq){
        return i;
    }

    double toffset = sqrt(rsq - dsq) / vec2_length(r->direction);

    if (i.type == INTERSECT_INTERIOR){
        i.distance = tc + toffset;
    } else {
        i.type = INTERSECT_EXTERIOR;
        i.distance = tc - toffset;
    }

    i.type = INTERSECT_VALID;
    i.point = vec2_add(r->origin, vec2_mul(r->direction, i.distance));
    double angle = vec2_angle(vec2_sub(i.point, a->origin), (vec2){1,0});    // Get angle of intersection
    // If angle is outside of arc, return no intersection
    if (angle < a->start_angle || angle > a->end_angle){
        i.type = INTERSECT_NONE;
        return i;
    }
    i.normal = vec2_normalize(vec2_sub(i.point, a->origin));

    return i;
}