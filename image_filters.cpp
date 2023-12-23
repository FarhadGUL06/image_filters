#include "image_filters.hpp"

const int filterHeight = 11;
const int filterWidth = 11;

double compute_factor(double copy[filterHeight][filterWidth]) {
    int sum = 0;
    for (int i = 0; i < filterHeight; i++) {
        for (int j = 0; j < filterWidth; j++) {
            sum += copy[i][j];
        }
    }
    return 1.0 / sum;
}

PPMImage *blur_PPM(PPMImage *image) {
    int bias = 0;
    double factor;
    double **filter = (double **)malloc(filterHeight * sizeof(double *));
    for (int i = 0; i < filterHeight; i++) {
        filter[i] = (double *)malloc(filterWidth * sizeof(double));
    }
    double copy[filterHeight][filterWidth] = {
        {0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0},
        {0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0}, {0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0},
        {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0}, {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0}, {0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0},
        {0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0}, {0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0},
    };

    factor = compute_factor(copy);

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
    int bias = 0;
    double factor = 1.0;
    double **filter = (double **)malloc(filterHeight * sizeof(double *));
    for (int i = 0; i < filterHeight; i++) {
        filter[i] = (double *)malloc(filterWidth * sizeof(double));
    }
    double copy[filterHeight][filterWidth] = {
        {1, 4, 8, 10, 10, 8, 4, 1, 0, 0, 0},
        {4, 16, 32, 40, 40, 32, 16, 4, 0, 0, 0},
        {8, 32, 64, 80, 80, 64, 32, 8, 0, 0, 0},
        {10, 40, 80, 100, 100, 80, 40, 10, 0, 0, 0},
        {10, 40, 80, 100, 100, 80, 40, 10, 0, 0, 0},
        {8, 32, 64, 80, 80, 64, 32, 8, 0, 0, 0},
        {4, 16, 32, 40, 40, 32, 16, 4, 0, 0, 0},
        {1, 4, 8, 10, 10, 8, 4, 1, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    };

    factor = compute_factor(copy);

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
    int bias = 0;
    double factor;
    double **filter = (double **)malloc(filterHeight * sizeof(double *));
    for (int i = 0; i < filterHeight; i++) {
        filter[i] = (double *)malloc(filterWidth * sizeof(double));
    }
    double copy[filterHeight][filterWidth] = {
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    };

    factor = compute_factor(copy);

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
    int bias = 0;
    double factor = 1.0;
    double **filter = (double **)malloc(filterHeight * sizeof(double *));
    for (int i = 0; i < filterHeight; i++) {
        filter[i] = (double *)malloc(filterWidth * sizeof(double));
    }
    double copy[filterHeight][filterWidth] = {
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, -1, -1, -1, 0, 0, 0, 0},
        {0, 0, 0, 0, -1, 9, -1, 0, 0, 0, 0},
        {0, 0, 0, 0, -1, -1, -1, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
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
    int bias = 0;
    double factor = 1.0;
    double **filter = (double **)malloc(filterHeight * sizeof(double *));
    for (int i = 0; i < filterHeight; i++) {
        filter[i] = (double *)malloc(filterWidth * sizeof(double));
    }
    double copy[filterHeight][filterWidth] = {
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0},
        {0, 0, 0, 0, 1, -7, 1, 0, 0, 0, 0},
        {0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
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
    int bias = 0;
    double factor = 1.0;
    double **filter = (double **)malloc(filterHeight * sizeof(double *));
    for (int i = 0; i < filterHeight; i++) {
        filter[i] = (double *)malloc(filterWidth * sizeof(double));
    }
    double copy[filterHeight][filterWidth] = {
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, -1, -1, -1, 0, 0, 0, 0},
        {0, 0, 0, 0, -1, 8, -1, 0, 0, 0, 0},
        {0, 0, 0, 0, -1, -1, -1, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
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
    double copy[filterHeight][filterWidth] = {-1, -1, 0, -1, 0, 1, 0, 1, 1};

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
    double copy[filterHeight][filterWidth] = {-1, -1, -1, -1, 0, -1, -1, -1, 0,
                                              1,  -1, -1, 0,  1, 1,  -1, 0,  1,
                                              1,  1,  0,  1,  1, 1,  1};

    for (int i = 0; i < filterHeight; i++) {
        for (int j = 0; j < filterWidth; j++) {
            filter[i][j] = copy[i][j];
        }
    }
    PPMImage *result =
        apply_filter(image, filter, filterHeight, filterWidth, factor, bias);
    return result;
}

PPMImage *random_PPM(PPMImage *image) {
    int bias = 0;
    double factor = 1.0;
    double **filter = (double **)malloc(filterHeight * sizeof(double *));
    for (int i = 0; i < filterHeight; i++) {
        filter[i] = (double *)malloc(filterWidth * sizeof(double));
    }
    double copy[filterHeight][filterWidth] = {
         {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
        {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
        {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
        {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
        {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
        {-1, -1, -1, -1, -1, 121, -1, -1, -1, -1, -1},
        {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
        {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
        {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
        {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
        {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
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

/*
PPMImage *resize_PPM(PPMImage *image, int height, int width) {
    PPMImage *result = (PPMImage *)malloc(sizeof(PPMImage));
    result->width = width;
    result->height = height;
    result->data = (PPMPixel *)malloc(result->width * result->height *
sizeof(PPMPixel)); for (int i = 0; i < result->height; i++) { for (int j = 0; j
< result->width; j++) { int index_i = i * image->height / result->height; int
index_j = j * image->width / result->width; result->data[j * result->height +
i].red = image->data[index_j * image->height + index_i].red; result->data[j *
result->height + i].green = image->data[index_j * image->height +
index_i].green; result->data[j * result->height + i].blue = image->data[index_j
* image->height + index_i].blue;
        }
    }
    return result;
}
*/