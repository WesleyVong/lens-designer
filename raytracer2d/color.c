#include "color.h"
#include "math.h"

// Convert wavelength to CIE XYZ based on CIE approximation
// Piecewise gaussian as described on 
// https://en.wikipedia.org/wiki/CIE_1931_color_space#Color_matching_functions
double cie_gaussian(double x, double mu, double t1, double t2){
    double result;
    if (x < mu){
        result =  exp(-(t1 * t1) * (x - mu) * (x - mu) * 0.5);
    } else {
        result =  exp(-(t2 * t2) * (x - mu) * (x - mu) * 0.5);
    }
    return result;
}

Color wavelength_to_color(double wavelength, ColorSpace color_space){
    // Clamp wavelength to visible range
    if (wavelength < 380 || wavelength > 780){
        return (Color){COLOR_LINEAR, 0, 0, 0, 1};
    }
    Color c;
    c.color_space = COLOR_XYZ;
    // Approximate 1931 color matching functions
    c.r = 1.056 * cie_gaussian(wavelength, 599.8, 0.0264, 0.0323) 
        + 0.362 * cie_gaussian(wavelength, 442, 0.0624, 0.0374)
        + -0.065 * cie_gaussian(wavelength, 501.1, 0.0490, 0.0382);
    c.g = 0.821 * cie_gaussian(wavelength, 568.8, 0.0213, 0.0247) 
        + 0.286 * cie_gaussian(wavelength, 530.9, 0.0613, 0.0322);
    c.b = 1.217 * cie_gaussian(wavelength, 437, 0.0845, 0.0278) 
        + 0.681 * cie_gaussian(wavelength, 459, 0.0385, 0.0725);
    c.a = 1;
    switch (color_space){
        case COLOR_LINEAR:
            to_linear(&c);
            break;
        case COLOR_SRGB:
            to_srgb(&c);
            break;
        case COLOR_XYZ:
            break;
        default:
            break;
    }
    return c;
}

// A bunch of color space conversion
// Conversion to and from CIE XYZ color space is based on 
// https://en.wikipedia.org/wiki/SRGB#Correspondence_to_CIE_XYZ_stimulus

void to_srgb(Color * c){
    switch(c->color_space){
        case COLOR_LINEAR:
            // Convert linear space to srgb
            if (c->r <= 0.0031308){
                c->r = 12.92 * c->r;
            } else {
                c->r = 1.055 * pow(c->r, 1.0 / 2.4) - 0.055;
            }
            if (c->g <= 0.0031308){
                c->g = 12.92 * c->g;
            } else {
                c->g = 1.055 * pow(c->g, 1.0 / 2.4) - 0.055;
            }
            if (c->b <= 0.0031308){
                c->b = 12.92 * c->b;
            } else {
                c->b = 1.055 * pow(c->b, 1.0 / 2.4) - 0.055;
            }
            break;
        case COLOR_SRGB:
            break;
        case COLOR_XYZ:
            // Convert XYZ to linear
            to_linear(c);
            // Recursive call but now color is in linear space
            to_srgb(c);
            break;
        default:
            return;
    }
    c->color_space = COLOR_SRGB;
    return;
}

void to_linear(Color * c){
    switch(c->color_space){
        case COLOR_LINEAR:
            break;
        case COLOR_SRGB:
            // Convert srgb to linear space
            if (c->r <= 0.04045){
                c->r = c->r / 12.92;
            } else {
                c->r = pow((c->r + 0.055) / 1.055, 2.4);
            }
            if (c->g <= 0.04045){
                c->g = c->g / 12.92;
            } else {
                c->g = pow((c->g + 0.055) / 1.055, 2.4);
            }
            if (c->b <= 0.04045){
                c->b = c->b / 12.92;
            } else {
                c->b = pow((c->b + 0.055) / 1.055, 2.4);
            }
            break;
        case COLOR_XYZ:
            // Apply transformation matrix
            c->r = 3.2404542 * c->r - 1.5371385 * c->g - 0.4985314 * c->b;
            c->g = -0.9692660 * c->r + 1.8760108 * c->g + 0.0415560 * c->b;
            c->b = 0.0556434 * c->r - 0.2040259 * c->g + 1.0572252 * c->b;
            break;
        default:
            return;
    }
    c->color_space = COLOR_LINEAR;
    return;
}

void to_xyz(Color * c){
    switch (c->color_space){
        case COLOR_LINEAR:
            // Apply transformation matrix
            c->r = 0.4124564 * c->r + 0.3575761 * c->g + 0.1804375 * c->b;
            c->g = 0.2126729 * c->r + 0.7151522 * c->g + 0.0721750 * c->b;
            c->b = 0.0193339 * c->r + 0.1191920 * c->g + 0.9503041 * c->b;
            break;
        case COLOR_SRGB:
            // Linearize SRGB
            to_linear(c);
            // Recursive call but now color is in linear space
            to_xyz(c);
            break;
        case COLOR_XYZ:
            break;
        default:
            return;
    }
    c->color_space = COLOR_XYZ;
    return;
}