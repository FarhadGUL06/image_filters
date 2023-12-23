#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cstdint>

const uint8_t rgb_max_value = 255;

typedef struct {
    unsigned char red, green, blue;
} PPMPixel;

typedef struct {
    int height, width;
    PPMPixel *data;
} PPMImage;

// Headers for functions
// Utils
PPMImage *apply_filter(PPMImage *image, double **filter, int filterHeight, int filterWidth, double factor, double bias);
int max(int a, int b);
int min(int a, int b);

// PPM IO
PPMImage *read_PPM(const char *filename);
void write_PPM(const char *filename, PPMImage *img);

// Filters
PPMImage *blur_PPM(PPMImage *image);
PPMImage *gaussian_blur_PPM(PPMImage *image);
PPMImage *motion_blur_PPM(PPMImage *image);
PPMImage *sharpen_PPM(PPMImage *image);
PPMImage *extreme_sharpen_PPM(PPMImage *image);
PPMImage *find_edges_PPM(PPMImage *image);
PPMImage *emboss_PPM(PPMImage *image);
PPMImage *extreme_emboss_PPM(PPMImage *image);
PPMImage *random_PPM(PPMImage *image);

// Array containing filters and id for each filter
static PPMImage *(*filters[])(PPMImage *) = {
    blur_PPM,
    gaussian_blur_PPM,
    motion_blur_PPM,
    sharpen_PPM,
    extreme_sharpen_PPM,
    find_edges_PPM,
    emboss_PPM,
    extreme_emboss_PPM,
    random_PPM
};
