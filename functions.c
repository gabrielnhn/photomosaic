#include "functions.h"
#include <math.h>


void bad_malloc()
{
    fprintf(stderr, "Error: bad memory allocation\n");
    exit(1);
}

void bad_filename(char filename[])
{
    fprintf(stderr, "Error: bad filename %s\n", filename);
    exit(1);
}

char* file_path(char dir[], char file[])
{
    // char point_slash[] = "./";
    char slash[] = "/";

    char* file_path_str= (char*) malloc (sizeof(char) * LINE_MAX);
    
    // strcpy(file_path_str, point_slash);
    strcpy(file_path_str, dir);
    strcat(file_path_str, slash); 

    strcat(file_path_str, file);

    return file_path_str;
}

image_t* new_image(int width, int height) 
{
    image_t* image = (image_t*) malloc(sizeof(image_t));
    image->width = width;
    image->height = height;

    image->pixels = (pixel_t**) malloc(image->height * sizeof(pixel_t*));
    if (!image->pixels)
        bad_malloc();


    for(int y = 0; y < image->height; y++)
    {
        image->pixels[y] = (pixel_t*) malloc(image->width * sizeof(pixel_t));
        if (!image->pixels[y])
            bad_malloc();
    }

    return image;
}

// pair_t filename_to_size(char* filename)
// // parse PPM file and get the image size (assuming it is a square)
// {
//     // fprintf(stderr, "getting size from %s\n", filename);
//     FILE* file;

//     if (strcmp(filename, "STDIN") == 0)
//         file = stdin;
//     else
//         file = fopen(filename, "r");
    
//     // fprintf(stderr, "file is %p", file);

//     if (file == NULL)
//         bad_filename(filename);


//     char line[LINE_MAX];
//     fgets(line, LINE_MAX, file);


//     int w,h;

//     int ret = sscanf(line, "%d %d", &w, &h);
//     while(ret != 2 && !feof(file)) // while didn't read width-height
//     {   
//         fgets(line, LINE_MAX, file);
//         ret = sscanf(line, "%d %d", &w, &h);
//     }

//     fclose(file);

//     if (ret != 2)
//     {
//         fprintf(stderr, "Error: Could not get image size in %s\n", filename);
//         exit(1);
//     }
//     else
//     {
//         pair_t pair;
//         pair.width = w;
//         pair.height = h;
//         return pair;
//     }
// }

image_t* filename_to_image(char* filename, pair_t *size, int* image_type)
{
    FILE* file;

    if (strcmp(filename, "STDIN") == 0)
        file = stdin;
    else
        file = fopen(filename, "r");
    
    if (!file)
        bad_filename(filename);

    
    *image_type = 0;

    char line[LINE_MAX];
    
    // get image type (P3 or P6)
    
    int ret = fscanf(file, "%s", line);
    if (ret == 1)
    {
        if (strcmp(line, "P3") == 0)
            *image_type = 3;
        else
        {
            if (strcmp(line, "P6") == 0) // sanity check
                *image_type = 6;
        }
    }
    if (*image_type != 3 && *image_type != 6)
    {
        fprintf(stderr, "Error parsing file %s\n", filename);
        exit(1);
    }

    // get image height/width (again)
    int has_read_sizes = 0;


    while(!has_read_sizes)
    {
        fgets(line, LINE_MAX, file);
        // printf("%s", line);

        // printf("Second line:%s", line);
        if (strncmp(line, "#", 1) != 0) // line not a comment
        {
            int ret = sscanf(line, "%d %d", &(size->width), &(size->height));
            // printf("2ndLine ret == %d\n", ret);
            if (ret == 2)
                has_read_sizes = 1;
            // printf("values: %d\n", has_read_sizes);
        }
    }

    image_t *image = new_image(size->width, size->height);
    
    // get maxvalue
    int has_read_maxvalue = 0;
    while(!feof(file) && !has_read_maxvalue)
    {
        int unused_variable;
        int ret = fscanf(file, "%d\n", &unused_variable);
        if (ret == 1)
            has_read_maxvalue = 1;
    
        // printf("maxvalue is %d", unused_variable);
    }

    // read them pixels
    if (*image_type == 3)
    {
        for (int y = 0; y < image->height; y++)
            for(int x = 0; x < image->width; x++)
            {
                fscanf(file, "%i %i %i", &(image->pixels[y][x].r), &(image->pixels[y][x].g), &(image->pixels[y][x].b));
            }
    }

    else if (*image_type == 6) // again, sanity check
    {
        // every time we multiply by 3 is because each pixel is (r, g, b).
        unsigned char buffer[3]; // store each byte
        
        for (int y = 0; y < image->height; y++)
        {
            // printf("reading pixels from line %d\n", y);
            for(int x = 0; x < image->width; x++)
            {
                fread(buffer, sizeof(unsigned char), 3, file);

                image->pixels[y][x].r = buffer[0];
                image->pixels[y][x].g = buffer[1];
                image->pixels[y][x].b = buffer[2];
            }
        }
        fgetc(file);
    }

    if (strcmp(filename, "STDIN") != 0)
        fclose(file);

    return image;
}

void print_image(image_t* image)
{
    // printf("Printing image\n");
    // printf("w, h: %d %d\n", image->width, image->height);
    for (int y = 0; y < image->height; y++)
    {
        for(int x = 0; x < image->width; x++)
        {
            printf("%d.%d.%d ", image->pixels[y][x].r, image->pixels[y][x].g, image->pixels[y][x].b);
        }
        printf("\n");
    }
}

void print_pixel(pixel_t pixel)
{
    printf("%d %d %d\n", pixel.r, pixel.g, pixel.b);
}

pixel_t calculate_predom_colour(image_t* image, pair_t start_coord, pair_t end_coord)
// calculate the predominant colour of a chunk of `image`, defined by the coordinates.
// shoutouts to https://sighack.com/post/averaging-rgb-colors-the-right-way
{
    int red_s = 0;
    int green_s = 0;
    int blue_s = 0;
    int pixels_n = 0;
    pixel_t pixel;

    for(int y = start_coord.height; y < end_coord.height; y++)
    {
        for(int x = start_coord.width; x < end_coord.width; x++)
        {
            pixel = image->pixels[y][x];


            red_s += pixel.r * pixel.r;
            green_s += pixel.g * pixel.g;
            blue_s += pixel.b * pixel.b;

            pixels_n++;
        }
    }

    pixel.r = sqrt(red_s/pixels_n);
    pixel.g = sqrt(green_s/pixels_n);
    pixel.b = sqrt(blue_s/pixels_n);

    return pixel;

}

double colour_difference(pixel_t p, pixel_t q)
{
    double r_prime = (p.r + q.r) / 2;

    double difference;
    difference = sqrt( (2 + (r_prime/256))*(p.r - q.r)*(p.r - q.r)  + 4*(p.g - q.g)*(p.g - q.g) + (2 + ((255 -r_prime)/256))*(p.b - q.b)*(p.b - q.b) );

    return difference;
}

void replace_chunk(image_t* to_fill, image_t* filler, pair_t start, pair_t end)
{
    // printf("start.height + filler->height - 1 < to_fill->height == %d\n", start.height + filler->height - 1 < to_fill->height);
    // if ((start.height + filler->height - 1 < to_fill->height) == 0)
    //     printf("%d + %d -1 < %d\n", start.height, filler->height, to_fill->height);

    int y = 0;
    for (int i = start.height; i < end.height; i++)
    {
        int x = 0;
        for (int j = start.width; j < end.width ; j++)
        {
            to_fill->pixels[i][j] = filler->pixels[y][x];
            x++;
        }
        y++;
    }
}

void free_image(image_t* image)
{
    for(int i = 0; i < image->height; i++)
    {
        free(image->pixels[i]);
    }

    free(image->pixels);

    free(image);

}




void write_image(image_t* image, char* filename, int image_type)
{
    FILE* file;

    if (strcmp(filename, "STDOUT") == 0)
        file = stdout;
    else
        file = fopen(filename, "w");

   // header

    if (image_type == 3)
        fputs("P3\n", file);
    else
        fputs("P6\n", file);

    // width, height
    fprintf(file, "%d %d\n", image->width, image->height);

    //maxvalue
    fprintf(file, "%d\n", 255);

    if (image_type == 3)
    {
        for (int i = 0; i < image->height; i++)
        {
             for (int j = 0; j < image->width; j++)
            {
                fprintf(file, "%d %d %d ", image->pixels[i][j].r, image->pixels[i][j].g, image->pixels[i][j].b);
            }
            fprintf(file, "\n");

        }
    }
    else // p6
    {
        char buffer[3];
        for (int i = 0; i < image->height; i++)
        {
            for (int j = 0; j < image->width; j++)
            {
                buffer[0] = image->pixels[i][j].r;
                buffer[1] = image->pixels[i][j].g;
                buffer[2] = image->pixels[i][j].b ; 

                fwrite(buffer, sizeof(char), 3, file);
            }

            // fputs("\n", file);

        }
    }

}
