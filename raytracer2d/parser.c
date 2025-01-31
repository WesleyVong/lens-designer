#include "parser.h"
#include "stdio.h"
#include "string.h"
#include "stdlib.h"

#define MAX_LINE_LENGTH 1024

typedef enum {
    CMD_NONE,
    CMD_IMAGE
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
    char * delimiter = " ";

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
    while (fgets(line, MAX_LINE_LENGTH, f)){
        printf("Parsing line %d\r\n", line_num);
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
            default:
                break;
        }
        line_num++;
    }
}