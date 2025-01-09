#include "image.h"
#include "raytracer2d.h"
#include "lodepng.h"
#include "object.h"
#include "stdio.h"
#include "ior.h"
#include "math.h"

int main(){
    Image * img = image_init(1920, 1080);
    img->scale = 0.1;

    Ray2d * r = ray2d_init(RAY_PRIMARY, (vec2){0, 0}, (vec2){1, 0}, 450, 1, NULL);
    Ray2d * r2 = ray2d_init(RAY_PRIMARY, (vec2){0, 0}, (vec2){1, 0}, 650, 1, NULL);

    Sellmeier s;
    s.num_coeffs = 3;
    s.b = (double[]){1.03961212, 0.231792344, 1.01046945};
    s.c = (double[]){0.006000699, 0.020017914, 103.560653};

    Material m;
    m.diffuse = (Color){0.5, 0.5, 0.5, 1, COLOR_SRGB};
    m.ior = s;

    double angle = 75;
    double rads = angle * 3.1415926535 / 180;
    Wall * w = wall_init(0, "Test wall", &m, (vec2){1, 0}, (vec2){cos(rads), sin(rads) }, 1);

    Raytracer2d * rt = raytracer2d_init(1, 10);
    raytracer2d_add_object(rt, (Object2d *) w);
    raytrace(rt, r);
    raytrace(rt, r2);

    draw_ray_path(img, r);
    draw_ray_path(img, r2);
    draw_object(img, w);

    lodepng_encode32_file("image.png", img->image, img->width, img->height);

    double wavelengths[3] = {380, 550, 780};
    for (long i = 0; i < 3; i++){
        double ior = sellmeier(&s, wavelengths[i]);
        printf("Wavelength: %f, IOR: %f\r\n", wavelengths[i], ior);
    }

    image_free(img);
}
