#ifndef COLOR_H
#define COLOR_H

typedef enum {
    COLOR_LINEAR,
    COLOR_SRGB,
    COLOR_XYZ   // CIE XYZ Color space
} ColorSpace;

typedef struct{
    double r;
    double g;
    double b;
    double a;
    ColorSpace color_space;
} Color;

Color wavelength_to_color(double wavelength, ColorSpace color_space);
void to_srgb(Color * c);
void to_linear(Color * c);
void to_xyz(Color * c);

#endif