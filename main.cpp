#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    // parse options

    char tiles_dir[100];
    scanf("tiles", tiles_dir);

    int input_is_stdin = 1;
    int output_is_stdout = 1;

    FILE *input;
    FILE *output;

    int c = getopt(argc, argv, "iop:");
    while ( c != -1)
    {
        switch (c)
        {
            case 'i': // update input method
                input_is_stdin = 0;
                char *input_file = optarg;
                break;
            case 'o': // update output method
                output_is_stdout = 0;
                char *output_file = optarg;
                break;
            case 'p': // update 'tiles' directory path
                char *new_tiles_dir = optarg;
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


    tiles = [];
    // get tiles
    open(tiles_dir)
        open(file1 in tiles)
    {
        size = get_imagesize();
    }
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