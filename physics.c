#include "physics.h"

double lambert(vec3 normal, vec3 light_dir){
    double lambert = vec3_dot(normal, light_dir);
    if (lambert < 0){
        lambert = 0;
    }
    return lambert;
}