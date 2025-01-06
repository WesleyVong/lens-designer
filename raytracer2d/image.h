#ifndef IMAGE_H
#define IMAGE_H

#include "raytracer2d.h"
#include "color.h"

typedef struct {
    long width;
    long height;

    ColorSpace color_space;

    vec2 origin;

    unsigned char * image;
    double scale;
    double t_step;
} Image;

void image_init(Image * img, long width, long height);
void image_free(Image * img);

void draw_ray(Image * img, Ray2d * r);
void draw_object(Image * img, void * obj);
void draw_pixel(Image * img, vec2 position, Color c);
void draw_line(Image * img, vec2 start, vec2 end, Color c);

#endif