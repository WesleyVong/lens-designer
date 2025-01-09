#ifndef IOR_H
#define IOR_H

#include "vecmath.h"

typedef struct {
    long num_coeffs;
    double * b;
    double * c;
} Sellmeier;

double sellmeier(Sellmeier * s, double wavelength);

#endif