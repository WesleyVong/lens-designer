#include "image.h"
#include "raytracer2d.h"
#include "lodepng.h"
#include "object.h"
#include "stdio.h"
#include "ior.h"

int main(){
    Image img;
    image_init(&img, 200, 200);
    img.scale = 1;

    Ray2d * r = ray2d_init(RAY_PRIMARY, (vec2){0, 0}, (vec2){1, 1}, 550, 1);

    Material m;
    m.diffuse = (Color){1, 0, 1, 1, COLOR_SRGB};

    Wall * w = wall_init(0, "Test wall", &m, (vec2){0, 0}, (vec2){-0.1, 1}, 0.2);

    draw_ray(&img, r);
    draw_line(&img, (vec2){75, 10}, (vec2){75, 120}, (Color){1, 0, 0, 1});
    draw_object(&img, w);

    lodepng_encode32_file("image.png", img.image, img.width, img.height);

    double b[3] = {1.03961212, 0.231792344, 1.01046945};
    double c[3] = {0.00600069867, 0.0200179144, 103.560653};
    Sellmeier s = (Sellmeier){3, b, c};

    double wavelengths[3] = {380, 550, 780};
    for (long i = 0; i < 3; i++){
        double ior = sellmeier(&s, wavelengths[i]);
        printf("Wavelength: %f, IOR: %f\r\n", wavelengths[i], ior);
    }
}
