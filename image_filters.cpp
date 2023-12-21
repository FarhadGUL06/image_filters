#include "image_filters.hpp"

PPMImage *apply_filter(PPMImage *image, double **filter, int filterHeight, int filterWidth, double factor, double bias) {
    PPMImage *result = (PPMImage *) malloc(sizeof(PPMImage));
    result->width = image->width;
    result->height = image->height;
    result->data = (PPMPixel *) malloc(result->width * result->height * sizeof(PPMPixel));
    for (int i = 0; i < image->height; i++) {
        for (int j = 0; j < image->width; j++) {
            double red = 0;
            double green = 0;
            double blue = 0;
            for (int k = 0; k < filterHeight; k++) {
                for (int l = 0; l < filterWidth; l++) {
                    int index_i = (i - filterHeight / 2 + k + image->height) % image->height;
                    int index_j = (j - filterWidth / 2 + l + image->width) % image->width;
                    red += image->data[index_j * image->height + index_i].red * filter[k][l];
                    green += image->data[index_j * image->height + index_i].green * filter[k][l];
                    blue += image->data[index_j * image->height + index_i].blue * filter[k][l];
                }
            }
            result->data[j * image->height + i].red = min(max(int(factor * red + bias), 0), 255);
            result->data[j * image->height + i].green = min(max(int(factor * green + bias), 0), 255);
            result->data[j * image->height + i].blue = min(max(int(factor * blue + bias), 0), 255);
        }
    }
    return result;
}

PPMImage *read_PPM(const char *filename) {
    char buff[16];
    PPMImage *img;
    FILE *fp;
    int c, rgb_comp_color;

    // open PPM file for reading
    fp = fopen(filename, "rb");
    if (!fp) {
        fprintf(stderr, "Unable to open file '%s'\n", filename);
        exit(1);
    }

    // read image format
    if (!fgets(buff, sizeof(buff), fp)) {
        perror(filename);
        exit(1);
    }

    // check the image format
    if (buff[0] != 'P' || buff[1] != '6') {
        fprintf(stderr, "Invalid image format (must be 'P6')\n");
        exit(1);
    }

    // alloc memory form image
    img = (PPMImage *)malloc(sizeof(PPMImage));
    if (!img) {
        fprintf(stderr, "Unable to allocate memory\n");
        exit(1);
    }

    // check for comments
    c = getc(fp);
    while (c == '#') {
        while (getc(fp) != '\n')
            ;
        c = getc(fp);
    }

    ungetc(c, fp);
    // read image size information
    if (fscanf(fp, "%d %d", &img->height, &img->width) != 2) {
        fprintf(stderr, "Invalid image size (error loading '%s')\n", filename);
        exit(1);
    }

    // read rgb component
    if (fscanf(fp, "%d", &rgb_comp_color) != 1) {
        fprintf(stderr, "Invalid rgb component (error loading '%s')\n",
                filename);
        exit(1);
    }

    // check rgb component depth
    if (rgb_comp_color != RGB_COMPONENT_COLOR) {
        fprintf(stderr, "'%s' does not have 8-bits components\n", filename);
        exit(1);
    }

    while (fgetc(fp) != '\n')
        ;
    // memory allocation for pixel data
    img->data = (PPMPixel *)malloc(img->height * img->width * sizeof(PPMPixel));

    if (!img) {
        fprintf(stderr, "Unable to allocate memory\n");
        exit(1);
    }

    // read pixel data from file
    if (fread(img->data, 3 * img->height, img->width, fp) != img->width) {
        fprintf(stderr, "Error loading image '%s'\n", filename);
        exit(1);
    }

    fclose(fp);
    return img;
}

void write_PPM(const char *filename, PPMImage *img) {
    FILE *fp;
    // open file for output
    fp = fopen(filename, "wb");
    if (!fp) {
        fprintf(stderr, "Unable to open file '%s'\n", filename);
        exit(1);
    }

    // write the header file
    // image format
    fprintf(fp, "P6\n");

    // image size
    fprintf(fp, "%d %d\n", img->height, img->width);

    // rgb component depth
    fprintf(fp, "%d\n", RGB_COMPONENT_COLOR);

    // pixel data
    fwrite(img->data, 3 * img->height, img->width, fp);
    fclose(fp);
}

int max(int a, int b) {
    return a > b ? a : b;
}

int min(int a, int b) {
    return a < b ? a : b;
}

double fmax(double a, double b) {
    return a > b ? a : b;
}

double fmin(double a, double b) {
    return a < b ? a : b;
}
