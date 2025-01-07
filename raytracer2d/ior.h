#ifndef IOR_H
#define IOR_H

#include "vecmath.h"

typedef struct {
    long num_coeffs;
    double * b;
    double * c;
} Sellmeier;

double sellmeier(Sellmeier * s, double wavelength);

double get_refraction_angle(double angle, double ior1, double ior2);

#endif