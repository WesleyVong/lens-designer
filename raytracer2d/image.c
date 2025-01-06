#include "image.h"
#include "math.h"


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

void image_init(Image * img, long width, long height){
    img->width = width;
    img->height = height;
    img->color_space = COLOR_SRGB;
    // Set Image origin to center of Image
    img->origin = (vec2){width / 2, height / 2};
    img->scale = 1;
    img->t_step = 0.1;
    img->image = (unsigned char *)malloc(width * height * 4 * sizeof(unsigned char));
    // Clear Image by setting RGB to 0 and Alpha to 255
    for (long i = 0; i < width * height * 4; i+=4){
        img->image[i] = 0;
        img->image[i + 1] = 0;
        img->image[i + 2] = 0;
        img->image[i + 3] = 255;
    }
}

void draw_ray(Image * img, Ray2d * r){
    // Calculate start pixel of ray
    long long_edge = img->width > img->height ? img->width : img->height;
    vec2 start = to_pixel(img, r->origin);
    // Calculate end pixel of ray
    // Multiply long edge by sqrt(2) in case of diagonal ray
    vec2 ray_direction = r->direction;
    ray_direction.y = -ray_direction.y; // Invert y axis to match Image coordinates
    vec2 end = vec2_add(start, vec2_mul(ray_direction, long_edge * 1.414));  
    Color ray_color = wavelength_to_color(r->wavelength, img->color_space);
    draw_line(img, start, end, ray_color);
}

void draw_object(Image * img, void * obj){
    // Draw object shape
    Object2d * o = (Object2d *)obj;
    vec2 start_point = o->get_point(o, 0);
    vec2 end_point;
    for (double t = img->t_step; t <= 1; t += img->t_step){
        end_point = o->get_point(o, t);
        draw_line(img, to_pixel(img, start_point), to_pixel(img, end_point), o->mat->diffuse);
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
    long index = (long)(round(pixel.y) * img->width + round(pixel.x)) * 4;
    img->image[index] = (unsigned char)(c.r * 255);
    img->image[index + 1] = (unsigned char)(c.g * 255);
    img->image[index + 2] = (unsigned char)(c.b * 255);
    img->image[index + 3] = (unsigned char)(c.a * 255);
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
}