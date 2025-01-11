#ifndef OBJECT_H
#define OBJECT_H

#include "vecmath.h"
#include "ray2d.h"
#include "surface.h"

typedef struct {
    long id;
    char * name;
    long num_surfaces;
    Surface2d ** surfaces;
    Material ** materials;
} Object2d;


#endif