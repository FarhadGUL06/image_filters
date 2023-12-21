#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct {
    unsigned char red, green, blue;
} PPMPixel;

typedef struct {
    int height, width;
    PPMPixel *data;
} PPMImage;

// Headers for functions
PPMImage *apply_filter(PPMImage *image, double **filter, int filterHeight, int filterWidth, double factor, double bias);
PPMImage *read_PPM(const char *filename);
void write_PPM(const char *filename, PPMImage *img);
int max(int a, int b);
int min(int a, int b);
double fmax(double a, double b);
double fmin(double a, double b);

#define RGB_COMPONENT_COLOR 255