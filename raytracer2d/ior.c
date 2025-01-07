#include "ior.h"
#include "math.h"

double sellmeier(Sellmeier * s, double wavelength){
    double result = 1;
    double lambda = wavelength / 1000; // Lambda is in micrometers, wavelength is in nanometers
    double lambda_sq = lambda * lambda;
    for (long i = 0; i < s->num_coeffs; i++){
        double numerator = s->b[i] * lambda_sq;
        double denominator = lambda_sq - s->c[i];
        result += numerator / denominator;
    }
    return sqrt(result);
}

double get_refraction_angle(double angle, double ior1, double ior2){
    // Based on snell's law
    double sin_refraction_angle = (ior1 / ior2) * sin(angle);
    double refraction_angle = arcsin(sin_refraction_angle);
    return refraction_angle;
}