#include "functions.h"

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

pair_t filename_to_size(char* filename)
// parse PPM file and get the image size (assuming it is a square)
{
    FILE* file;

    file = fopen(filename, "r");
    if (!file)
        bad_filename(filename);


    char line[LINE_MAX];
    fgets(line, LINE_MAX, file);


    int w,h;

    int ret = sscanf(line, "%d %d", &w, &h);
    while(ret != 2 && !feof(file)) // while didn't read width-height
    {   
        fgets(line, LINE_MAX, file);
        ret = sscanf(line, "%d %d", &w, &h);
    }

    fclose(file);

    if (ret != 2)
    {
        fprintf(stderr, "Error: Could not get image size in %s\n", filename);
        exit(1);
    }
    else
    {
        pair_t pair;
        pair.width = w;
        pair.height = h;
        return pair;
    }
}

image_t* filename_to_image(char* filename, pair_t size)
{

    FILE* file;

    file = fopen(filename, "r");
    if (!file)
        bad_filename(filename);

    image_t *image = new_image(size.width, size.height);
    
    int image_type = 0;

    char line[LINE_MAX];
    char line_buffer[LINE_MAX];
    
    while(!feof(file) && image_type == 0)
    // get image type (P3 or P6)
    {
        fgets(line_buffer, LINE_MAX, file);
        sscanf(line_buffer, "%s", line);
        // printf("%s", line);
        if (strncmp(line, "#", 1) != 0) // line not a comment
        {

            if (strcmp(line, "P3") == 0)
                image_type = 3;
            else
            {
                if (strcmp(line, "P6") == 0) // sanity check
                    image_type = 6;
                else
                    image_type = -1;
            }
        }
    }
    printf("image_type == %d\n", image_type);
    if (image_type != 3 && image_type != 6)
    {
        fprintf(stderr, "Error parsing file %s\n", filename);
        exit(1);
    }

    // get image height/width (again)
    int has_read_sizes = 0;
    int unused_variable;


    while(!feof(file) && !has_read_sizes)
    {
        fgets(line, LINE_MAX, file);
        
        printf("Second line:%s", line);
        if (strncmp(line, "#", 1) != 0) // line not a comment
        {
            int ret = sscanf(line, "%d %d", &unused_variable, &unused_variable);
            printf("2ndLine ret == %d\n", ret);
            if (ret == 2)
                has_read_sizes = 1;
            
        }
    }

    // get maxvalue
    int has_read_maxvalue = 0;
    while(!feof(file) && !has_read_maxvalue)
    {
        int ret = fscanf(file, "%d\n", &unused_variable);
        if (ret == 1)
            has_read_maxvalue = 1;
    
        printf("maxvalue is %d", unused_variable);
    }

    // read them pixels
    if (image_type == 3)
    {
        for (int y = 0; y < image->height; y++)
            for(int x = 0; x < image->width; x++)
            {
                fscanf(file, "%d %d %d", &(image->pixels[y][x].r), &(image->pixels[y][x].g), &(image->pixels[y][x].b));
            }
    }

    else if (image_type == 6) // again, sanity check
    {
        // every time we multiply by 3 is because each pixel is (r, g, b).
        char* buffer = (char*) malloc (image->width * 3 * sizeof(char) + 4); // store each line
        if (!buffer)
            bad_malloc();

        for (int y = 0; y < image->height; y++)
        {
            printf("reading pixels from line %d\n", y);
            fread(buffer, sizeof(char) * 3, image->width, file);
            for(int x = 0; x < image->width; x++)
            {
                printf("storing pixel %d-%d: ", y, x);
                printf("%d %d %d\n", buffer[3*x], buffer[3*x + 1], buffer[3*x + 2]);
                image->pixels[y][x].r = buffer[3*x];
                image->pixels[y][x].g = buffer[3*x + 1];
                image->pixels[y][x].b = buffer[3*x + 2];
            }
        }
        printf("Image read.\n");
    }


    return image;
}

void print_image(image_t* image)
{
    printf("Printing image\n");
    printf("w, h: %d %d\n", image->width, image->height);
    for (int y = 0; y < image->height; y++)
    {
        for(int x = 0; x < image->width; x++)
        {
            printf("%d%d%d ", image->pixels[y][x].r, image->pixels[y][x].g, image->pixels[y][x].b);
        }
        printf("\n");
    }
}