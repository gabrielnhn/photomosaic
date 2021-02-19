#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

#define LINE_MAX 100 // maximum amount of chars per line

typedef struct
{
    int r, g, b;
} pixel_t;

typedef struct
{
    int width, height;
    pixel_t **pixels;
} image_t;

typedef struct
{
    int width, height;
} pair_t;


void bad_malloc();
// exit program on bad_malloc()

void bad_filename(char filename[]);
// exit program on bad filename (file not found)

char *file_path(char dir[], char file[]);
// return a string with the path to `file` in `dir` 

image_t *new_image(int width, int height);
// allocate enough memory for an image with such . return it as a pointer

image_t *filename_to_image(char *filename, pair_t *size, int *image_type);
// from `filename`, parse a '.ppm' file (P3 or P6) and
// store its content in an `image_t`. return it as pointer.
// one may also get some parameters through `size` and `image_type` (P3 or P6)

void print_pixel(pixel_t pixel);
// print pixel in 'stdin'

pixel_t calculate_predom_colour(image_t *image, pair_t start_coord, pair_t end_coord);
// calculate the predominant colour of a chunk of `image`, defined by the coordinates.
// return it as a pixel
// shoutouts to https://sighack.com/post/averaging-rgb-colors-the-right-way

double colour_difference(pixel_t p, pixel_t q);
// calculate the difference between colours using "redmean"

void replace_chunk(image_t *to_fill, image_t *filler, pair_t start, pair_t end);
// replace a chunk of an image `to_fill` with a `filler` image, from `start` to `end`.

void free_image(image_t *image);
// opposite of `new_image()`, deallocate the memory used by the image.

void write_image(image_t *image, char *filename, int image_type);
// write `image` to a `filename`, according to `image_type` (3 -> P3 and 6 -> P6)