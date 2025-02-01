#include "image.h"
#include "math.h"
#include "surface.h"
#include "stdlib.h"

void draw_surface(Image * img, Surface2d * s, Color c);

vec2 to_pixel(Image * img, vec2 pix){
    long long_edge = img->width > img->height ? img->width : img->height;
    // Invert y axis
    pix.y = -pix.y;
    // Scale by Image pixels and Image scale factor
    pix = vec2_mul(pix, long_edge / 2);
    pix = vec2_mul(pix, img->scale);
    // Center to origin
    pix = vec2_add(pix, img->origin);
    return pix;
}

Image * image_init(long width, long height){
    Image * img = malloc(sizeof(Image));
    img->width = width;
    img->height = height;
    img->color_space = COLOR_SRGB;
    img->draw_mode = COLOR_ADD;
    // Set Image origin to center of Image
    img->origin = (vec2){width / 2, height / 2};
    img->scale = 1;
    img->t_step = 0.1;
    img->image = calloc(width * height * 4, sizeof(unsigned char));
    img->name = NULL;
    return img;
}

void draw_raytracer(Image * img, Raytracer2d * rt){
    for (long r = 0; r < rt->num_rays; r++){
        draw_ray_path(img, rt->rays[r]);
    }
    for (long s = 0; s < rt->num_surfaces; s++){
        Material * m = (s == 0) ? rt->materials[s] : rt->materials[s-1];
        Color c = m->diffuse;
        switch (img->color_space){
            case COLOR_SRGB:
                to_srgb(&c);
                break;
            case COLOR_LINEAR:
                to_linear(&c);
                break;
        }
        draw_surface(img, rt->surfaces[s], c);
    }
}

void draw_ray_path(Image * img, Ray2d * r){
    Ray2d * current_ray = r;
    while (current_ray != NULL){
        draw_ray(img, current_ray);
        current_ray = current_ray->refracted_rays;
    }
}

void draw_ray(Image * img, Ray2d * r){
    // Calculate start pixel of ray
    long long_edge = img->width > img->height ? img->width : img->height;
    vec2 origin = (vec2){r->origin.x, r->origin.y}; // Negate the y axis to fit canvas orientation
    vec2 ray_direction = (vec2){r->direction.x, r->direction.y};   // Negate the y axis to fit canvas orientation
    vec2 start = to_pixel(img, origin);
    // Calculate end pixel of ray
    vec2 end;
    if (r->intersection->type == INTERSECT_NONE){
            end = vec2_add(origin, vec2_mul(ray_direction, long_edge * 1.414));  // Multiplied long edge by 1.414 to fully fit diagonal (fix)
    } else {
        end = vec2_add(origin, vec2_mul(ray_direction, r->intersection->distance));
    }
    end = to_pixel(img, end);
    Color ray_color = wavelength_to_color(r->wavelength, img->color_space);
    // Multiply ray color by intensity
    ray_color.r *= r->intensity;
    ray_color.g *= r->intensity;
    ray_color.b *= r->intensity;
    draw_line(img, start, end, ray_color);
}

void draw_surface(Image * img, Surface2d * s, Color c){
    // Draw surface
    vec2 start_point = s->get_point(s, 0);
    vec2 end_point;
    for (double t = img->t_step; t <= 1; t += img->t_step){
        end_point = s->get_point(s, t);
        vec2 line_start = to_pixel(img, start_point);
        vec2 line_end = to_pixel(img, end_point);
        draw_line(img, line_start, line_end, c);
        start_point = end_point;
    }
}

void draw_pixel(Image * img, vec2 pixel, Color c){
    // Clip color to 0 and 1
    c.r = c.r < 0 ? 0 : c.r > 1 ? 1 : c.r;
    c.g = c.g < 0 ? 0 : c.g > 1 ? 1 : c.g;
    c.b = c.b < 0 ? 0 : c.b > 1 ? 1 : c.b;
    // Check if position is within Image bounds
    if (pixel.x < 0 || pixel.x >= img->width || pixel.y < 0 || pixel.y >= img->height){
        return;
    }
    // Draw pixel
    long index = (long)(floor(pixel.y) * img->width + floor(pixel.x)) * 4;
    if (img->draw_mode == COLOR_OVERRIDE){
        
    }
    switch (img->draw_mode){
        case COLOR_OVERRIDE:
            img->image[index] = (unsigned char)(c.r * 255);
            img->image[index + 1] = (unsigned char)(c.g * 255);
            img->image[index + 2] = (unsigned char)(c.b * 255);
            img->image[index + 3] = (unsigned char)(c.a * 255);
            break;
        case COLOR_ADD:
            if (0xff - img->image[index] >= (unsigned char)(c.r * 255)){
                img->image[index] += (unsigned char)(c.r * 255);
            } else {
                img->image[index] = 0xff;
            }
            if (0xff - img->image[index + 1] >= (unsigned char)(c.g * 255)){
                img->image[index + 1] += (unsigned char)(c.g * 255);
            } else {
                img->image[index + 1] = 0xff;
            }
            if (0xff - img->image[index + 2] >= (unsigned char)(c.b * 255)){
                img->image[index + 2] += (unsigned char)(c.b * 255);
            } else {
                img->image[index + 2] = 0xff;
            }
            if (0xff - img->image[index + 3] >= (unsigned char)(c.a * 255)){
                img->image[index + 3] += (unsigned char)(c.a * 255);
            } else {
                img->image[index + 3] = 0xff;
            }
    }

}

void draw_line(Image *img, vec2 start, vec2 end, Color c){
    // Clip color to 0 and 1
    c.r = c.r < 0 ? 0 : c.r > 1 ? 1 : c.r;
    c.g = c.g < 0 ? 0 : c.g > 1 ? 1 : c.g;
    c.b = c.b < 0 ? 0 : c.b > 1 ? 1 : c.b;

    double dx = end.x - start.x;
    double dy = end.y - start.y;

    if (dx == 0 && dy == 0){
        draw_pixel(img, start, c);
        return;
    }

    // By default step along x axis
    // If x is zero, then step along y axis
    long num_steps;
    vec2 step;
    if (fabs(dy) > fabs(dx)){
        step = (vec2){dx / fabs(dy), dy / fabs(dy)};
        num_steps = fabs(round(dy));
    }
    else {
        step = (vec2){dx / fabs(dx), dy / fabs(dx)};
        num_steps = fabs(round(dx));
    }
    for (long s = 0; s < num_steps; s++){
        vec2 position = vec2_add(start, vec2_mul(step, s));
        draw_pixel(img, position, c);
    }
}

void image_free(Image * img){
    free(img->image);
    free(img);
}