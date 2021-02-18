#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#define LINE_MAX 100 // maximum amount of chars per file line

typedef struct 
{
    int r, g, b; 
} pixel_t;


typedef struct 
{
    int width, height;
    pixel_t** pixels;
} image_t;

typedef struct
{
    int width, height;
} pair_t;



void bad_malloc();

void bad_filename(char filename[]);

char* file_path(char dir[], char file[]);

image_t* new_image(int width, int height); 

pair_t filename_to_size(char* filename);

image_t* filename_to_image(char* filename, pair_t size);

void print_image(image_t* image);
