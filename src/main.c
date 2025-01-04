#include "stdio.h"
#include "raytracer.h"
#include "lodepng.h"
#include "math.h"
#include "vecmath.h"
#include "time.h"

int main(){
    // Generate blank image
    long width = 500, height = 500;
    unsigned char* image = (unsigned char*)malloc(width * height * 4);
    for(int i = 0; i < width * height * 4; i++){
        image[i] = 0;
    }
    // Create raytracer
    raytracer rt;
    rt.width = width;
    rt.height = height;
    rt.max_bounces = 10;
    rt.rays_per_pixel = 1;
    rt.num_objects = 2;
    rt.objects = (object*)malloc(rt.num_objects * sizeof(object));
    rt.num_emitters = 1;
    rt.emitters = (emitter*)malloc(rt.num_emitters * sizeof(emitter));

    // Create spheres
    // sphere s1 = {0, 4, -2, 1};
    sphere s1 = {2, -1, 0, 2};
    material m1 = {{1, 1, 1}, .8};
    object o1 = {OBJECT_SPHERE, 0, &s1, &m1};
    rt.objects[0] = o1;

    // sphere s2 = {0, 4, 0, 0.5};
    sphere s2 = {-2, -1, 0, 2};
    material m2 = {{1, 1, 1}, .8};
    object o2 = {OBJECT_SPHERE, 1, &s2, &m2};
    rt.objects[1] = o2;

    // Create emitter
    sun sun = {{0, 1, 1.2}};
    material sun_mat = {{1, 1, 1}};
    emitter e = {EMIT_SUN, &sun, &sun_mat};
    rt.emitters[0] = e;

    // Create rays
    vec3 eye = {0, 0, 0};
    vec3 forward = {0, -1, 0};
    vec3 up = {0, 0, 1};
    generate_rays(&rt, eye, forward, up);

    // Raytrace image
    clock_t start_time = clock();
    raytrace_image(&rt, image);
    clock_t end_time = clock();
    double elapsed_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;
    printf("Raytrace time: %f seconds\n", elapsed_time);


    // Save to PNG using lodepng
    lodepng_encode32_file("3d.png", image, width, height);
    // Free memory
    raytrace_free(&rt);
    free(image);
    printf("Done\n");
    return 0;
    
}