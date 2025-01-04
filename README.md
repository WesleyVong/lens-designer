# Lens Designer

The lens designer has two ray tracing enviroments. A side profile 2d raytracer that shows the behavior of a group of lens elements and a 3d raytracer that emulates the lens behavior in the real world.

The goal of the raytracers is to simulate accurate light refraction throughout the glass optics. This is done by using parametric equations for the optics and simulating multiple rays for different wavelengths of light.

## Implementation

### 2D Raytracer

A sequential raytracer that assumes 100% transparency in the optics.

### 3D Raytracer

Basic non-sequential raytracer that supports roughness, transparency, and reflections.

To increase speed, the initial rays out of the camera and lens is calculated based on the 2D raytracer and rays are extrapolated to 3d by rotating the 2D rays around the camera axis