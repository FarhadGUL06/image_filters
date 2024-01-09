
#include "image_filters.hpp"

PPMImage *apply_filter(PPMImage *image, double **filter, int filterHeight,
                       int filterWidth, double factor, double bias) {
    PPMImage *result = (PPMImage *)malloc(sizeof(PPMImage));
    result->width = image->width;
    result->height = image->height;
    result->data =
        (PPMPixel *)malloc(result->width * result->height * sizeof(PPMPixel));

    for (int i = 0; i < image->height; i++) {
        for (int j = 0; j < image->width; j++) {
            double red = 0;
            double green = 0;
            double blue = 0;

            for (int k = 0; k < filterHeight; k++) {
                for (int l = 0; l < filterWidth; l++) {
                    int index_i = i - filterHeight / 2 + k;
                    int index_j = j - filterWidth / 2 + l;

                    if (index_i >= 0 && index_i < image->height &&
                        index_j >= 0 && index_j < image->width) {
                        red +=
                            image->data[index_j * image->height + index_i].red *
                            filter[k][l];
                        green += image->data[index_j * image->height + index_i]
                                     .green *
                                 filter[k][l];
                        blue += image->data[index_j * image->height + index_i]
                                    .blue *
                                filter[k][l];
                    }
                }
            }

            result->data[j * image->height + i].red =
                min(max((int)(factor * red + bias), 0), 255);
            result->data[j * image->height + i].green =
                min(max((int)(factor * green + bias), 0), 255);
            result->data[j * image->height + i].blue =
                min(max((int)(factor * blue + bias), 0), 255);
        }
    }
    return result;
}

int max(int a, int b) { return a > b ? a : b; }

int min(int a, int b) { return a < b ? a : b; }
