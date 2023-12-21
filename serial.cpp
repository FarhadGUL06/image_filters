#include <stdio.h>

#include "image_filters.hpp"

PPMImage *blur_PPM(PPMImage *image) {
    const int filterHeight = 5;
    const int filterWidth = 5;

    int bias = 0;
    double factor = 1.0 / 13.0;
    double **filter = (double **)malloc(filterHeight * sizeof(double *));
    for (int i = 0; i < filterHeight; i++) {
        filter[i] = (double *)malloc(filterWidth * sizeof(double));
    }
    double copy[filterHeight][filterWidth] = {
        0, 0, 1, 0, 0, 
        0, 1, 1, 1, 0, 
        1, 1, 1, 1, 1, 
        0, 1, 1, 1, 0, 
        0, 0, 1, 0, 0,
    };

    for (int i = 0; i < filterHeight; i++) {
        for (int j = 0; j < filterWidth; j++) {
            filter[i][j] = copy[i][j];
        }
    }
    PPMImage *result =
        apply_filter(image, filter, filterHeight, filterWidth, factor, bias);
    return result;
}

PPMImage *gaussian_blur_PPM(PPMImage *image) {
    const int filterHeight = 5;
    const int filterWidth = 5;

    int bias = 0;
    double factor = 1.0 / 273.0;
    double **filter = (double **)malloc(filterHeight * sizeof(double *));
    for (int i = 0; i < filterHeight; i++) {
        filter[i] = (double *)malloc(filterWidth * sizeof(double));
    }
    double copy[filterHeight][filterWidth] = {
        1,  4,  7,  4,  1, 
        4,  16, 26, 16, 4, 
        7,  26, 41, 26, 7, 
        4,  16, 26, 16, 4, 
        1,  4,  7,  4,  1,
    };

    for (int i = 0; i < filterHeight; i++) {
        for (int j = 0; j < filterWidth; j++) {
            filter[i][j] = copy[i][j];
        }
    }
    PPMImage *result =
        apply_filter(image, filter, filterHeight, filterWidth, factor, bias);
    return result;
}

PPMImage *motion_blur_PPM(PPMImage *image) {
    const int filterHeight = 9;
    const int filterWidth = 9;

    int bias = 0;
    double factor = 1.0 / 9.0;
    double **filter = (double **)malloc(filterHeight * sizeof(double *));
    for (int i = 0; i < filterHeight; i++) {
        filter[i] = (double *)malloc(filterWidth * sizeof(double));
    }
    double copy[filterHeight][filterWidth] = {
        1, 0, 0, 0, 0, 0, 0, 0, 0, 
        0, 1, 0, 0, 0, 0, 0, 0, 0, 
        0, 0, 1, 0, 0, 0, 0, 0, 0, 
        0, 0, 0, 1, 0, 0, 0, 0, 0, 
        0, 0, 0, 0, 1, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 1, 0, 0, 0, 
        0, 0, 0, 0, 0, 0, 1, 0, 0, 
        0, 0, 0, 0, 0, 0, 0, 1, 0, 
        0, 0, 0, 0, 0, 0, 0, 0, 1,
    };

    for (int i = 0; i < filterHeight; i++) {
        for (int j = 0; j < filterWidth; j++) {
            filter[i][j] = copy[i][j];
        }
    }
    PPMImage *result =
        apply_filter(image, filter, filterHeight, filterWidth, factor, bias);
    return result;
}

PPMImage *sharpen_PPM(PPMImage *image) {
    const int filterHeight = 3;
    const int filterWidth = 3;

    int bias = 0;
    double factor = 1.0;
    double **filter = (double **)malloc(filterHeight * sizeof(double *));
    for (int i = 0; i < filterHeight; i++) {
        filter[i] = (double *)malloc(filterWidth * sizeof(double));
    }
    double copy[filterHeight][filterWidth] = {
  -1, -1, -1,
  -1,  9, -1,
  -1, -1, -1
    };

    for (int i = 0; i < filterHeight; i++) {
        for (int j = 0; j < filterWidth; j++) {
            filter[i][j] = copy[i][j];
        }
    }
    PPMImage *result =
        apply_filter(image, filter, filterHeight, filterWidth, factor, bias);
    return result;
}


PPMImage *extreme_sharpen_PPM(PPMImage *image) {
    const int filterHeight = 3;
    const int filterWidth = 3;

    int bias = 0;
    double factor = 1.0;
    double **filter = (double **)malloc(filterHeight * sizeof(double *));
    for (int i = 0; i < filterHeight; i++) {
        filter[i] = (double *)malloc(filterWidth * sizeof(double));
    }
    double copy[filterHeight][filterWidth] = {
  1,  1,  1,
   1, -7,  1,
   1,  1,  1
    };

    for (int i = 0; i < filterHeight; i++) {
        for (int j = 0; j < filterWidth; j++) {
            filter[i][j] = copy[i][j];
        }
    }
    PPMImage *result =
        apply_filter(image, filter, filterHeight, filterWidth, factor, bias);
    return result;
}



PPMImage *find_edges_PPM(PPMImage *image) {
    const int filterHeight = 5;
    const int filterWidth = 5;

    int bias = 0;
    double factor = 1.0;
    double **filter = (double **)malloc(filterHeight * sizeof(double *));
    for (int i = 0; i < filterHeight; i++) {
        filter[i] = (double *)malloc(filterWidth * sizeof(double));
    }
    double copy[filterHeight][filterWidth] = {
  -1, -1, -1, -1, -1,
  -1,  2,  2,  2, -1,
  -1,  2,  8,  2, -1,
  -1,  2,  2,  2, -1,
  -1, -1, -1, -1, -1,
    };

    for (int i = 0; i < filterHeight; i++) {
        for (int j = 0; j < filterWidth; j++) {
            filter[i][j] = copy[i][j];
        }
    }
    PPMImage *result =
        apply_filter(image, filter, filterHeight, filterWidth, factor, bias);
    return result;
}

PPMImage *emboss_PPM(PPMImage *image) {
    const int filterHeight = 3;
    const int filterWidth = 3;

    int bias = 128;
    double factor = 1.0;
    double **filter = (double **)malloc(filterHeight * sizeof(double *));
    for (int i = 0; i < filterHeight; i++) {
        filter[i] = (double *)malloc(filterWidth * sizeof(double));
    }
    double copy[filterHeight][filterWidth] = {
  -1, -1,  0,
  -1,  0,  1,
   0,  1,  1
    };

    for (int i = 0; i < filterHeight; i++) {
        for (int j = 0; j < filterWidth; j++) {
            filter[i][j] = copy[i][j];
        }
    }
    PPMImage *result =
        apply_filter(image, filter, filterHeight, filterWidth, factor, bias);
    return result;
}

PPMImage *extreme_emboss_PPM(PPMImage *image) {
    const int filterHeight = 5;
    const int filterWidth = 5;

    int bias = 128;
    double factor = 1.0;
    double **filter = (double **)malloc(filterHeight * sizeof(double *));
    for (int i = 0; i < filterHeight; i++) {
        filter[i] = (double *)malloc(filterWidth * sizeof(double));
    }
    double copy[filterHeight][filterWidth] = {
  -1, -1, -1, -1,  0,
  -1, -1, -1,  0,  1,
  -1, -1,  0,  1,  1,
  -1,  0,  1,  1,  1,
   0,  1,  1,  1,  1
    };

    for (int i = 0; i < filterHeight; i++) {
        for (int j = 0; j < filterWidth; j++) {
            filter[i][j] = copy[i][j];
        }
    }
    PPMImage *result =
        apply_filter(image, filter, filterHeight, filterWidth, factor, bias);
    return result;
}

PPMImage *resize_PPM(PPMImage *image, int height, int width) {
    PPMImage *result = (PPMImage *)malloc(sizeof(PPMImage));
    result->width = width;
    result->height = height;
    result->data = (PPMPixel *)malloc(result->width * result->height * sizeof(PPMPixel));
    for (int i = 0; i < result->height; i++) {
        for (int j = 0; j < result->width; j++) {
            int index_i = i * image->height / result->height;
            int index_j = j * image->width / result->width;
            result->data[j * result->height + i].red = image->data[index_j * image->height + index_i].red;
            result->data[j * result->height + i].green = image->data[index_j * image->height + index_i].green;
            result->data[j * result->height + i].blue = image->data[index_j * image->height + index_i].blue;
        }
    }
    return result;
}



int main(int argc, char *argv[]) {
    // Arguments: input_file and output_file
    if (argc != 3) {
        printf("Usage: %s <input_file> <output_file>\n", argv[0]);
        exit(1);
    }

    char *input_file = argv[1];
    char *output_file = argv[2];

    PPMImage *image;
    image = read_PPM(input_file);
    printf("Image size: %d x %d\n", image->width, image->height);

    PPMImage *result = resize_PPM(image, image->height / 2, image->width / 2);
    PPMImage *result2 = resize_PPM(result, image->height * 2, image->width * 2);

    write_PPM(output_file, result);
    return 0;
}
