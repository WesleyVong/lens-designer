#ifndef IMAGE_H
#define IMAGE_H

#include "raytracer2d.h"
#include "color.h"

typedef enum {
    COLOR_OVERRIDE,
    COLOR_ADD
} DrawMode;

typedef struct {
    unsigned char * name;

    long width;
    long height;

    ColorSpace color_space;
    DrawMode draw_mode;

    vec2 origin;

    unsigned char * image;
    double scale;
    double t_step;
} Image;

Image * image_init(long width, long height);
void image_free(Image * img);

void draw_raytracer(Image * img, Raytracer2d * rt);
void draw_ray_path(Image * img, Ray2d * r);
void draw_ray(Image * img, Ray2d * r);
void draw_surface(Image * img, Surface2d * s, Color c);
void draw_pixel(Image * img, vec2 position, Color c);
void draw_line(Image * img, vec2 start, vec2 end, Color c);

#endif