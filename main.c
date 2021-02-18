#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h> // using isprint()
#include <stdlib.h>
#include <dirent.h> // accessing directories

#include "functions.h"

int main(int argc, char *argv[])
{
    // parse options

    int input_is_stdin = 1;
    int output_is_stdout = 1;
    int tiles_dir_is_default = 1;

    char *input_file;
    char *output_file;
    char* tiles_dir;

    int c = getopt(argc, argv, "iop:");
    while ( c != -1)
    {
        switch (c)
        {
            case 'i': // update input method
                input_is_stdin = 0;
                input_file = optarg;
                break;
            case 'o': // update output method
                output_is_stdout = 0;
                output_file = optarg;
                break;
            case 'p': // update 'tiles' directory path
                tiles_dir_is_default = 0;
                tiles_dir = optarg;
                break;
            case '?':
                if (optopt == 'i' || optopt == 'o' || optopt == 'p')
                    fprintf(stderr, "Option -%c requires an argument.\n", optopt);
                else if (isprint(optopt))
                    fprintf(stderr, "Unknown option `-%c'.\n", optopt);
                else
                    fprintf(stderr,"Unknown option character `\\x%x'.\n", optopt);
                return 1;
            default:
                abort();
        }
        c = getopt(argc, argv, "iop:");
    }

    if (tiles_dir_is_default)
        tiles_dir = "./tiles";

    fprintf(stderr, "Reading tiles from %s\n", tiles_dir);    

    // opening the tiles directory

    DIR *d;
    struct dirent *dir;
    d = opendir(tiles_dir);

    // determine the number of tiles
    int tiles_n = 0;
    while((dir = readdir(d)) != NULL) {
        // remember, when str1 == str2, strcmp == 0.
        if ( strcmp(dir->d_name, ".") && strcmp(dir->d_name, "..") )
            tiles_n++;
    }
    rewinddir(d);

    // get the size of the tiles 

    int valid_file = 0;
    int tile_size;

    while(((dir = readdir(d)) != NULL) && valid_file) {
        // remember, when str1 == str2, strcmp == 0.
        if ( strcmp(dir->d_name, ".") && strcmp(dir->d_name, "..") )
            valid_file = 1;
            tile_size = filename_to_size(dir->d_name);
    }

    // parse and store tile images

    image_t *tiles = (image_t*) malloc(sizeof(image_t) * tiles_n);

    int i = 0;
    while((dir = readdir(d)) != NULL) {
        if ( strcmp(dir->d_name, ".") && strcmp(dir->d_name, "..") )
        {
            tiles[i] = filename_to_image(dir->d_name);
            i++;
        }
    }

    // get tile data

    for (file in tiles_dir)
    {
        open(file)
            matrix = allocate_matrix(size);
        image_from_file(matrix, file);
        tiles.insert(matrix)
    }






    // calculate the predominant colour
    predominant_colours = [];
    for (tile in tiles)
    {
        colour = calculate_predom_colour(tile);
        predominant_colours.insert(colour);
    }

    // open main input file
    open(input)
        type = input.type;
    // split into chunks of $size
    for (pixel_chunk(size) in input)
    {
        chunk_colour = calculate_predom_colour(pizel_chunk)

            // find the tile with the closest predominant colour
            min = __INT_MAX__;
        for (colour in predominant_colours)
            if (colour_difference(chunk_colour, colour) < min)
                min = colour_difference(chunk_colour, colour)
                    min_index = colour_index;

        //replace chunk with the tile

        substitute(pixel_chunk, tiles[min_index]);

        // write it using the same type as the input
        save_image(output, type);
    }

    return 0;
}