#ifndef PARSER_H
#define PARSER_H

#include "raytracer2d.h"
#include "image.h"

void parse_file(char * fname, Image ** img, Raytracer2d ** rt);

#endif