#include <stdio.h>
#include <stdlib.h>

typedef struct {
    unsigned char red, green, blue;
} PPMPixel;

typedef struct {
    int x, y;
    PPMPixel *data;
} PPMImage;

// Headers for functions
PPMImage *readPPM(const char *filename);
void writePPM(const char *filename, PPMImage *img);

#define RGB_COMPONENT_COLOR 255