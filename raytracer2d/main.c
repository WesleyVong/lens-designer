#include "image.h"
#include "raytracer2d.h"
#include "lodepng.h"
#include "stdio.h"
#define _USE_MATH_DEFINES
#include "math.h"
#include "debug_tools.h"
#include "stdlib.h"
#include "parser.h"

int main(){
    StopwatchEntry * se = stopwatch_init();
    stopwatch_reset(se);

    Image * img;
    Raytracer2d * rt;

    parse_file("test_script.r2d", &img, &rt);

    img->origin = (vec2){img->width / 4, img->height / 2};
    img->color_space = COLOR_LINEAR;

    if (img->name == NULL){
        img->name = "image.png";
    }

    double angle = 15;
    double rads = angle * 3.1415926535 / 180;
    long num_wavelengths = 20;
    double minimum_wavelength = 400;
    double maximum_wavelength = 750;
    double wavelength_step = (maximum_wavelength - minimum_wavelength) / num_wavelengths;
    double intensity = 1;
    rt->num_rays = num_wavelengths;
    rt->rays = malloc(rt->num_rays * sizeof(Ray2d *));
    for (long i = 0; i < rt->num_rays; i++){
        double wavelength = minimum_wavelength + i * wavelength_step;
        rt->rays[i] = ray2d_init(RAY_PRIMARY, (vec2){0, 0.25}, (vec2){1,0}, wavelength, intensity, NULL);
    }

    Sellmeier s;
    s.num_coeffs = 3;
    s.b = (double[]){1.03961212, 0.231792344, 1.01046945};
    s.c = (double[]){0.006000699, 0.020017914, 103.560653};

    Material m;
    m.diffuse = (Color){0.5, 0.5, 0.5, 1, COLOR_SRGB};
    m.ior = s;

    Sellmeier air;
    air.num_coeffs = 0;

    Material m2;
    m2.diffuse = (Color){0.5, 0.5, 0.5, 1, COLOR_SRGB};
    m2.ior = air;

    angle = 60;
    rads = angle * M_PI / 180;
    Line * s1 = line_init((vec2){1, 0}, (vec2){cos(rads), sin(rads)}, 2);
    Line * s2 = line_init((vec2){2, 0}, (vec2){cos(-rads), sin(-rads)}, 2);

    angle = 30;
    rads = angle * M_PI / 180;
    Arc * a1 = arc_init((vec2){1.2, 0}, 1, -rads + M_PI, rads + M_PI);
    Arc * a2 = arc_init((vec2){-0.3, 0}, 1, -rads, rads);

    raytracer2d_add_surface(rt, (Surface2d *) a1);
    raytracer2d_add_surface(rt, (Surface2d *) a2);

    raytracer2d_add_surface(rt, (Surface2d *) s1);
    raytracer2d_add_surface(rt, (Surface2d *) s2);

    raytracer2d_add_material(rt, &m);
    raytracer2d_add_material(rt, &m2);
    raytracer2d_add_material(rt, &m);

    stopwatch_print_reset(se, "Initialize");

    raytrace(rt);
    stopwatch_print_reset(se, "Raytrace");

    draw_raytracer(img, rt);
    stopwatch_print_reset(se, "Draw");

    lodepng_encode32_file(img->name, img->image, img->width, img->height);
    stopwatch_print_reset(se, "Encode");

    image_free(img);
    stopwatch_free(se);
}
