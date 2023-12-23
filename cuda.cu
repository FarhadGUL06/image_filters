#include <cuda.h>
#include <cuda_runtime_api.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cstdint>


// Functions in this file

const uint8_t rgb_max_value = 255;
const int filterHeight = 11;
const int filterWidth = 11;

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

// Struct for payload
struct thread_payload {
    size_t thread_id;
    PPMImage **images;
    PPMImage *(**to_apply)(PPMImage *);
    int number_of_filters;
    int start;
    int end;
};

__global__ void thread_pixel(PPMPixel *d_data,
                             double *d_filter, int *d_fi, int *d_fj,
                            PPMPixel *d_result_data, double *d_factor,
                             double *d_bias, int *d_width, int *d_height) {

    unsigned int index = blockIdx.x * blockDim.x + threadIdx.x;

    if (index > *d_width * *d_height) {
        return;
    }

    int d_i = index % *d_width;
    int d_j = index / *d_width;

    double red = 0;
    double green = 0;
    double blue = 0;
    for (int k = 0; k < *d_fi; k++) {
        for (int l = 0; l < *d_fj; l++) {
            int index_i = d_i - *d_fi / 2 + k;
            int index_j = d_j - *d_fj / 2 + l;

            if (index_i >= 0 && index_i < *d_height && index_j >= 0 &&
                index_j <*d_width) {
                red += d_data[index_j *  *d_height + index_i].red *
                       d_filter[k * *d_fi + l];
                green +=
                    d_data[index_j * *d_height + index_i].green *
                    d_filter[k * *d_fi + l];
                blue +=
                    d_data[index_j * *d_height + index_i].blue *
                    d_filter[k * *d_fi + l];
                printf("%lf %lf %lf\n", red, green, blue);
            }
        }
    }
    d_result_data[d_j * *d_height + d_i].red =
        min(max((int)(*d_factor * red + *d_bias), 0), 255);
    d_result_data[d_j * *d_height + d_i].green =
        min(max((int)(*d_factor * green + *d_bias), 0), 255);
    d_result_data[d_j * *d_height + d_i].blue =
        min(max((int)(*d_factor * blue + *d_bias), 0), 255);
        
    //printf("All parameters: %d %d %d %d %d %d %lf %lf %d\n", d_i, d_j, *d_fi, *d_fj, *d_width, *d_height, *d_factor, *d_bias, index);
}

void read_all_images(PPMImage **images, int number_of_images) {
    for (int i = 0; i < number_of_images; i++) {
        char filename[50];
        sprintf(filename, "images/ppm_before/image%d.ppm", i);
        images[i] = read_PPM(filename);
    }
}

void write_all_images(PPMImage **images, int number_of_images) {
    for (int i = 0; i < number_of_images; i++) {
        char filename[50];
        sprintf(filename, "images/ppm_after/image%d.ppm", i);
        write_PPM(filename, images[i]);
    }
}

void free_all_images(PPMImage **images, int number_of_images,
                     PPMImage *(**to_apply)(PPMImage *)) {
    for (int i = 0; i < number_of_images; i++) {
        free(images[i]->data);
        free(images[i]);
    }
    free(images);
    free(to_apply);
}

PPMImage *(**read_all_filters(int *number_of_filters))(PPMImage *) {
    scanf("%d", number_of_filters);
    PPMImage *(**to_apply)(PPMImage *) = (PPMImage * (**)(PPMImage *))
        malloc(sizeof(PPMImage * (*)()) * (*number_of_filters + 1));

    for (int i = 0; i < *number_of_filters; i++) {
        int filter_id;
        scanf("%d", &filter_id);

        to_apply[i] = filters[filter_id];
    }

    return to_apply;
}


void apply_all_filters(PPMImage **images, int number_of_images,
                       PPMImage *(**to_apply)(PPMImage *),
                       int number_of_filters) {

    for (int i = 0; i < number_of_images; i++) {
        for (int j = 0; j < number_of_filters; j++) {
            images[i] = to_apply[j](images[i]);
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Usage: ./cuda <number_of_images> <num_threads>\n");
        return 1;
    }
    const int number_of_images = atoi(argv[1]);
    if (number_of_images <= 0 || number_of_images > 50) {
        printf("Invalid number of images\n");
        return 1;
    }

    int number_of_filters;
    PPMImage **images =
        (PPMImage **)malloc(sizeof(PPMImage *) * number_of_images);
    read_all_images(images, number_of_images);

    PPMImage *(**to_apply)(PPMImage *) = read_all_filters(&number_of_filters);

    apply_all_filters(images, number_of_images, to_apply, number_of_filters);

    write_all_images(images, number_of_images);
    free_all_images(images, number_of_images, to_apply);
    return 0;
}


PPMImage *apply_filter(PPMImage *image, double **filter, int filterHeight, int filterWidth, double factor, double bias) {
    PPMPixel *d_data;
    PPMPixel *d_result_data;
    double *d_filter;
    double *d_factor;
    double *d_bias;
    int *d_width;
    int *d_height;
    int *d_i;
    int *d_j;
    int *d_fi;
    int *d_fj;

    // cudaMalloc
    cudaMalloc(&d_data, sizeof(PPMPixel) * image->height * image->width);
    cudaMalloc(&d_result_data, sizeof(PPMPixel) * image->height * image->width);
    cudaMalloc(&d_factor, sizeof(double));
    cudaMalloc(&d_bias, sizeof(double));
    cudaMemset(d_factor, 0, sizeof(double));
    cudaMemset(d_bias, 0, sizeof(double));
    cudaMalloc(&d_width, sizeof(int));
    cudaMalloc(&d_height, sizeof(int));
    cudaMalloc(&d_i, sizeof(int));
    cudaMalloc(&d_j, sizeof(int));
    cudaMalloc(&d_fi, sizeof(int));
    cudaMalloc(&d_fj, sizeof(int));

    // cudaMemcpy
    printf("Bias and factor %f %f\n", bias, factor);
    cudaMemcpy(d_data, image->data, sizeof(PPMPixel) * image->height * image->width, cudaMemcpyHostToDevice);
    cudaMemcpy(d_factor, &factor, sizeof(double), cudaMemcpyHostToDevice);
    cudaMemcpy(d_bias, &bias, sizeof(double), cudaMemcpyHostToDevice);
    cudaMemcpy(d_width, &image->width, sizeof(int), cudaMemcpyHostToDevice);
    cudaMemcpy(d_height, &image->height, sizeof(int), cudaMemcpyHostToDevice);

    // d_filter combine them into one array
    double *filter_array = (double *)malloc(sizeof(double) * filterHeight * filterWidth);
    for (int i = 0; i < filterHeight; i++) {
        for (int j = 0; j < filterWidth; j++) {
            filter_array[i * filterHeight + j] = filter[i][j];
        }
    }
    cudaMalloc(&d_filter, sizeof(double) * filterHeight * filterWidth);
    cudaMemcpy(d_filter, filter_array, sizeof(double) * filterHeight * filterWidth, cudaMemcpyHostToDevice);

    // cudaMemsetNull
    cudaMemset(d_result_data, 0, sizeof(PPMPixel) * image->height * image->width);

    // Kernel
    int blockSize = 256;
    int numBlocks = 65535;
    
    for (int i = 0; i < image->height; i++) {
        for (int j = 0; j < image->width; j++) {
            cudaMemcpy(d_i, &i, sizeof(int), cudaMemcpyHostToDevice);
            cudaMemcpy(d_j, &j, sizeof(int), cudaMemcpyHostToDevice);
            cudaMemcpy(d_fi, &filterHeight, sizeof(int), cudaMemcpyHostToDevice);
            cudaMemcpy(d_fj, &filterWidth, sizeof(int), cudaMemcpyHostToDevice);
            thread_pixel<<<numBlocks, blockSize>>>(d_data, d_filter, d_fi, d_fj, d_result_data, d_factor, d_bias, d_width, d_height);
        }
    }

    // cudaMemcpy
    cudaMemcpy(image->data, d_result_data, sizeof(PPMPixel) * image->height * image->width, cudaMemcpyDeviceToHost);


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
    if (rgb_comp_color != rgb_max_value) {
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
    fprintf(fp, "%d\n", rgb_max_value);

    // pixel data
    fwrite(img->data, 3 * img->height, img->width, fp);
    fclose(fp);
}

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



