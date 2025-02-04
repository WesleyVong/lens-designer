#include "image.h"
#include "raytracer2d.h"
#include "lodepng.h"
#include "stdio.h"
#define _USE_MATH_DEFINES
#include "math.h"
#include "debug_tools.h"
#include "stdlib.h"
#include "parser.h"

int main(int argc, char ** argv){
    StopwatchEntry * se = stopwatch_init();
    stopwatch_reset(se);

    Image * img;
    Raytracer2d * rt;

    if (argc < 2){
        printf("Usage: %s path\\to\\file.r2d\n", argv[0]);
        return 1;
    }

    parse_file(argv[1], &img, &rt);

    // parse_file("test_script.r2d", &img, &rt);

    img->color_space = COLOR_LINEAR;

    if (img->name == NULL){
        img->name = "image.png";
    }

    stopwatch_print_reset(se, "Parse Time");

    draw_raytracer(img, rt);
    stopwatch_print_reset(se, "Draw Time");

    lodepng_encode32_file(img->name, img->image, img->width, img->height);
    printf("Wrote image to %s\n", img->name);
    stopwatch_print_reset(se, "Encode Time");

    raytracer2d_free_all(rt);
    image_free(img);
    stopwatch_free(se);
}
