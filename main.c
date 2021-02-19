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

    char input_file[LINE_MAX];
    char output_file[LINE_MAX];
    char *tiles_dir;

    opterr = 0;
    int c = getopt(argc, argv, "ho:p:i:");
    while (c != -1)
    {
        switch (c)
        {
        case 'h':
            fprintf(stderr, "This is a photomosaic generator.");
            fprintf(stderr, "This is a photomosaic generator.");
            exit(1);
            break;
        case 'o': // update output method
            output_is_stdout = 0;
            strncpy(output_file, optarg, LINE_MAX - 1);
            fprintf(stderr, "%s will be the output\n", optarg);
            break;
        case 'i': // update input method
            input_is_stdin = 0;
            strncpy(input_file, optarg, LINE_MAX - 1);
            fprintf(stderr, "%s will be the input\n", optarg);
            break;
        case 'p': // update 'tiles' directory path
            tiles_dir_is_default = 0;
            tiles_dir = optarg;
            fprintf(stderr, "%s will be the directory for the tiles\n", optarg);
            break;
        case ':':
        case '?':
            if (optopt == 'i' || optopt == 'o' || optopt == 'p')
                fprintf(stderr, "Option -%c requires an argument.\n", optopt);
            else if (isprint(optopt))
                fprintf(stderr, "Unknown option `-%c'.\n", optopt);
            else
                fprintf(stderr, "Unknown option character `\\x%x'.\n", optopt);
            return 1;
        default:
            abort();
        }
        c = getopt(argc, argv, "ho:p:i:");
    }

    if (tiles_dir_is_default)
        tiles_dir = "./tiles";

    fprintf(stderr, "Reading tiles from %s\n", tiles_dir);

    // opening the tiles directory

    DIR *d;
    struct dirent *dir;
    d = opendir(tiles_dir);
    if (!d)
    {
        fprintf(stderr, "Error opening 'tiles' directory\n");
        exit(1);
    }
    rewinddir(d);

    // determine the number of tiles
    int tiles_n = 0;
    while ((dir = readdir(d)) != NULL)
    {
        // remember, when str1 == str2, strcmp == 0.
        if (strcmp(dir->d_name, ".") && strcmp(dir->d_name, ".."))
            tiles_n++;
    }
    fprintf(stderr, "There are %d tiles in %s\n", tiles_n, tiles_dir);
    rewinddir(d);

    // get the size of the tiles

    int valid_file = 0;
    pair_t tile_size;

    while (((dir = readdir(d)) != NULL) && !valid_file)
    {
        if (strcmp(dir->d_name, ".") && strcmp(dir->d_name, ".."))
        // remember, when str1 == str2, strcmp() == 0.
        {
            valid_file = 1;

            char *file_path_str = file_path(tiles_dir, dir->d_name);
            tile_size = filename_to_size(file_path_str);
        }
    }
    rewinddir(d);

    fprintf(stderr, "Tile size is %d-%d\n", tile_size.width, tile_size.height);
    // parse and store tile images

    image_t **tiles = (image_t **)malloc(sizeof(image_t *) * tiles_n);

    int i = 0;
    int p3_n = 0;
    int p6_n = 0;
    int type = 0;
    while ((dir = readdir(d)) != NULL)
    {
        if (strcmp(dir->d_name, ".") && strcmp(dir->d_name, ".."))
        {
            char *file_path_str = file_path(tiles_dir, dir->d_name);
            tiles[i] = filename_to_image(file_path_str, tile_size, &type);


            i += 1;
            // fprintf(stderr, "i == %d\n", i);

            if (type == 3)
                p3_n++;
            else
                p6_n++;
        }
    }

    fprintf(stderr, "All %d tiles were stored successfully\n", tiles_n);
    fprintf(stderr, "%d P3 tiles and %d P6 tiles\n", p3_n, p6_n);

    // calculate the predominant colour of each tile
    pixel_t *predominant_colours = (pixel_t *)malloc(tiles_n * sizeof(pixel_t));

    for (int i = 0; i < tiles_n; i++)
    {
        pair_t start, end;
        start.width = 0;
        start.height = 0;
        end.width = tiles[i]->width;
        end.height = tiles[i]->height;

        pixel_t colour = calculate_predom_colour(tiles[i], start, end);
        predominant_colours[i] = colour;
    }

    fprintf(stderr, "Predominant colours were calculated for every tile.\n");
    // for (int i = 0; i < tiles_n; i++)
    //     print_pixel(predominant_colours[i]);

    // open main input file
    if (input_is_stdin)
        strcpy(input_file, "STDIN");

    fprintf(stderr, "Accessing input file...\n");
    fprintf(stderr, "File is: %s\n", input_file);
    int input_type = 0;
    pair_t input_size = filename_to_size(input_file);
    image_t *input_image = filename_to_image(input_file, input_size, &input_type);

    fprintf(stderr, "Image type is P%d\n", input_type);
    fprintf(stderr, "Image size is %dx%d\n", input_size.width, input_size.height);

    // print_image(input_image);
    // split input image into chunks of $tile_size

    fprintf(stderr, "Replacing chunks\n");

    for(int y = 0; y < input_image->height; y = y + tile_size.height)
    {
        for(int x = 0; x < input_image->width; x = x + tile_size.width)
        {
            pair_t chunk_start, chunk_end;
            chunk_start.height = y; chunk_start.width = x;
            chunk_end.height = y + tile_size.height - 1; chunk_end.width = x + tile_size.width - 1;

            // fprintf(stderr, "Replacing chunk from (%d,%d) to (%d, %d)\n", chunk_start.height, chunk_start.width, chunk_end.height, chunk_end.width);

            pixel_t chunk_colour = calculate_predom_colour(input_image, chunk_start, chunk_end);

            // find the tile whose predominant colour is the closest to the image chunk
            
            // assume its the first one:
            double min_diff = colour_difference(chunk_colour, predominant_colours[0]);
            int min_index = 0;
            // compare to the others
            for (int k= 1; k < tiles_n; k++)
            {
                double diff = colour_difference(chunk_colour, predominant_colours[k]);
                if (diff < min_diff)
                {
                    min_diff = diff;
                    min_index = k;
                }
            }

            // now we have the optimal tile!
            // let us replace the chunk.

            // fprintf(stderr, "index: %d\n", min_index);
            // print_image(tiles[min_index]);

            replace_chunk(input_image, tiles[min_index], chunk_start);
        }
    }


    // write the output image
    if (output_is_stdout)
        strcpy(output_file, "STDOUT");

    fprintf(stderr, "Writing image...\n");
    write_image(input_image, output_file, input_type);

    return 0;
}