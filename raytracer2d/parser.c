#include "parser.h"
#include "stdio.h"
#include "string.h"
#include "stdlib.h"

#define MAX_LINE_LENGTH 1024

typedef enum {
    CMD_NONE,
    CMD_IMAGE,
    CMD_IOR,
    CMD_MATERIAL,
    CMD_ARC,
    CMD_LINE,
    CMD_RAY
} CommandType;

typedef struct Command {
    CommandType keyword;
    long num_args;
    void ** args;
} Command;

Command * parse_line(char * line){
    Command * cmd = malloc(sizeof(Command));
    cmd->keyword = CMD_NONE;
    cmd->num_args = 0;
    cmd->args = NULL;

    char * token;
    char * delimiter = " \n";

    token = strtok(line, delimiter);
    while (token != NULL){
        if (cmd->keyword != CMD_NONE){
            if (cmd->args == 0){
                cmd->args = malloc(sizeof(void **));
            } else {
                cmd->args = realloc(cmd->args, (cmd->num_args + 1) * sizeof(void **));
            }
            cmd->num_args++;
        }
        switch (cmd->keyword){
            case CMD_NONE:
                if (strcmp(token, "IMAGE") == 0){
                    cmd->keyword = CMD_IMAGE;
                } else if (strcmp(token, "IOR") == 0){
                    cmd->keyword = CMD_IOR;
                } else if (strcmp(token, "MATERIAL") == 0){
                    cmd->keyword = CMD_MATERIAL;
                } else if (strcmp(token, "ARC") == 0){
                    cmd->keyword = CMD_ARC;
                } else if (strcmp(token, "LINE") == 0){
                    cmd->keyword = CMD_LINE;
                } else if (strcmp(token, "RAY") == 0){
                    cmd->keyword = CMD_RAY;
                }
                break;
            case CMD_IMAGE:
                switch (cmd->num_args-1){
                    case 2:
                        cmd->args[cmd->num_args-1] = (void *)malloc(sizeof(double));
                        *(double *)cmd->args[cmd->num_args-1] = strtod(token, NULL);
                        break;
                    case 3:
                        cmd->args[cmd->num_args-1] = (void *)malloc(strlen(token) * sizeof(char));
                        strcpy((char *)cmd->args[cmd->num_args-1], token);
                        break;
                    default:
                        cmd->args[cmd->num_args-1] = (void *)malloc(sizeof(long));
                        *(long *)cmd->args[cmd->num_args-1] = strtol(token, NULL, 10);
                        break;
                }
                break;
            case CMD_IOR:
                switch (cmd->num_args-1){
                    case 0:
                        cmd->args[cmd->num_args-1] = (void *)malloc(sizeof(long));
                        *(long *)cmd->args[cmd->num_args-1] = strtol(token, NULL, 10);
                        break;
                    default:
                        cmd->args[cmd->num_args-1] = (void *)malloc(sizeof(double));
                        *(double *)cmd->args[cmd->num_args-1] = strtod(token, NULL);
                        break;
                }
                break;
            case CMD_MATERIAL:
                cmd->args[cmd->num_args-1] = (void *)malloc(sizeof(double));
                *(double *)cmd->args[cmd->num_args-1] = strtod(token, NULL);
                break;
            case CMD_ARC:
                cmd->args[cmd->num_args-1] = (void *)malloc(sizeof(double));
                *(double *)cmd->args[cmd->num_args-1] = strtod(token, NULL);
                break;
            case CMD_LINE:
                cmd->args[cmd->num_args-1] = (void *)malloc(sizeof(double));
                *(double *)cmd->args[cmd->num_args-1] = strtod(token, NULL);
                break;
            case CMD_RAY:
                cmd->args[cmd->num_args-1] = (void *)malloc(sizeof(double));
                *(double *)cmd->args[cmd->num_args-1] = strtod(token, NULL);
                break;
            default:
                break;
        }
        token = strtok(NULL, delimiter);
    }
    return cmd;
}

void parse_file(char * fname, Image ** img, Raytracer2d ** rt){
    *rt = raytracer2d_init();
    // Open file with fname
    FILE * f = fopen(fname, "r");
    // Get every line in file
    char line[MAX_LINE_LENGTH];
    long line_num = 0;

    Sellmeier * ior;

    Material * m;

    while (fgets(line, MAX_LINE_LENGTH, f)){
        Command * cmd = parse_line(line);
        switch (cmd->keyword){
            case CMD_IMAGE:
                if (cmd->num_args < 2){
                    break;
                }
                long image_width = *(long *)cmd->args[0];
                long image_height = *(long *)cmd->args[1];
                *img = image_init(image_width, image_height);
                if (cmd->num_args >= 3){
                    double image_scale = *(double *)cmd->args[2];
                    (*img)->scale = image_scale;
                }
                if (cmd->num_args >= 4){
                    char * image_name = (char *)cmd->args[3];
                    (*img)->name = malloc(strlen(image_name) * sizeof(char));
                    strcpy((*img)->name, image_name);
                }
                break;
            case CMD_IOR:
                if (cmd->num_args < 1){
                    break;
                }
                long num_coeffs = *(long *)cmd->args[0];
                if (num_coeffs * 2 > cmd->num_args - 1){   // If the number of coefficients given is less than required
                    break;
                }
                ior = malloc(sizeof(Sellmeier));
                ior->num_coeffs = num_coeffs;
                ior->b = malloc(num_coeffs * sizeof(double));
                ior->c = malloc(num_coeffs * sizeof(double));
                for (long i = 0; i < num_coeffs; i++){
                    ior->b[i] = *(double *)cmd->args[i+1];
                    ior->c[i] = *(double *)cmd->args[i+1+num_coeffs];
                }
                break;
            case CMD_MATERIAL:
                if (cmd->num_args < 3){
                    break;
                }
                if (ior == NULL){
                    break;
                }
                m = malloc(sizeof(Material));
                m->ior = ior;
                if (cmd->num_args == 3){
                    m->diffuse = (Color){*(double *)cmd->args[0], *(double *)cmd->args[1], *(double *)cmd->args[2], 1, COLOR_SRGB};
                } else {
                    m->diffuse = (Color){*(double *)cmd->args[0], *(double *)cmd->args[1], *(double *)cmd->args[2], *(double *)cmd->args[3], COLOR_SRGB};
                }
                break;
            case CMD_ARC:
                if (cmd->num_args < 5){
                    break;
                }
                if (m == NULL){
                    break;
                }
                double arc_x = *(double *)cmd->args[0];
                double arc_y = *(double *)cmd->args[1];
                double arc_radius = *(double *)cmd->args[2];
                double arc_start = *(double *)cmd->args[3];
                double arc_end = *(double *)cmd->args[4];
                Arc * a = arc_init((vec2){arc_x, arc_y}, arc_radius, arc_start, arc_end);
                raytracer2d_add_surface(*rt, (Surface2d *) a);
                raytracer2d_add_material(*rt, m);
                break;
            case CMD_LINE:
                if (cmd->num_args < 5){
                    break;
                }
                if (m == NULL){
                    break;
                }
                vec2 line_origin = (vec2){*(double *)cmd->args[0], *(double *)cmd->args[1]};
                vec2 line_direction = (vec2){*(double *)cmd->args[2], *(double *)cmd->args[3]};
                double line_length = *(double *)cmd->args[4];
                Line * l = line_init(line_origin, line_direction, line_length);
                raytracer2d_add_surface(*rt, (Surface2d *) l);
                raytracer2d_add_material(*rt, m);
                break;
            case CMD_RAY:
                if (cmd->num_args < 6){
                    break;
                }
                double ray_x = *(double *)cmd->args[0];
                double ray_y = *(double *)cmd->args[1];
                double ray_dir_x = *(double *)cmd->args[2];
                double ray_dir_y = *(double *)cmd->args[3];
                double ray_wavelength = *(double *)cmd->args[4];
                double ray_intensity = *(double *)cmd->args[5];
                Ray2d * r = ray2d_init(RAY_PRIMARY, (vec2){ray_x, ray_y}, (vec2){ray_dir_x, ray_dir_y}, ray_wavelength, ray_intensity, NULL);
                raytracer2d_add_ray(*rt, r);
                break;
            default:
                break;
        }
        line_num++;
    }
}