#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <math.h>

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

image_t* filename_to_image(char* filename, pair_t size, int* image_type);

void print_image(image_t* image);

void print_pixel(pixel_t pixel);

pixel_t calculate_predom_colour(image_t* image, pair_t start_coord, pair_t end_coord);

double colour_difference(pixel_t p, pixel_t q);

void replace_chunk(image_t* to_fill, image_t* filler, pair_t start);

void write_image(image_t* image, char* filename, int image_type);