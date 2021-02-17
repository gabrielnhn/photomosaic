#include <stdio.h>

int main(int argc, char *argv[])
{
    // parse options
    // default:

    FILE* tiles_dir = "tiles";
    FILE* input = stdin;
    FILE* output = stdout;

    while (didnt parse)
    {
        if ("-p")
            tiles_dir = "tiles directory"
        else
        {
            if ("-i")
                input = "input file";
            else
                if ("-o")
                    output = "output file"
        }
    }


    tiles = [];
    // get tiles
    open(tiles_dir)
    open(file1 in tiles)
    {
        size = get_imagesize();
    }
    for(file in tiles_dir)
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
    for(pixel_chunk(size) in input)
    {
        chunk_colour = calculate_predom_colour(pizel_chunk)
        
        // find the tile with the closest predominant colour
        min = __INT_MAX__;
        for(colour in predominant_colours)
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