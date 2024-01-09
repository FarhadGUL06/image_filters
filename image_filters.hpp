/**
 * @file image_filters.hpp
 * @brief Header file containing functions for applying various filters to PPM images.
 */

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

/**
 * @brief Applies a filter to an image.
 *
 * This function applies a filter to the input image and returns a new PPMImage pointer
 * containing the filtered image.
 *
 * @param image The input image to apply the filter to.
 * @param filter The filter matrix.
 * @param filterHeight The height of the filter matrix.
 * @param filterWidth The width of the filter matrix.
 * @param factor The factor to scale the filtered values by.
 * @param bias The bias to add to the filtered values.
 * @return A new PPMImage pointer containing the filtered image.
 */
PPMImage *apply_filter(PPMImage *image, double **filter, int filterHeight, int filterWidth, double factor, double bias);

/**
 * @brief Finds the maximum of two integers.
 *
 * @param a The first integer.
 * @param b The second integer.
 * @return The maximum of the two integers.
 */
int max(int a, int b);

/**
 * @brief Finds the minimum of two integers.
 *
 * @param a The first integer.
 * @param b The second integer.
 * @return The minimum of the two integers.
 */
int min(int a, int b);

/**
 * @brief Reads a PPM image from a file.
 *
 * @param filename The name of the file to read the image from.
 * @return A pointer to the PPMImage struct containing the read image.
 */
PPMImage *read_PPM(const char *filename);

/**
 * @brief Writes a PPM image to a file.
 *
 * @param filename The name of the file to write the image to.
 * @param img A pointer to the PPMImage struct containing the image to write.
 */
void write_PPM(const char *filename, PPMImage *img);

/**
 * @brief Applies a blur filter to a PPM image.
 *
 * @param image The input image to apply the filter to.
 * @return A new PPMImage pointer containing the filtered image.
 */
PPMImage *blur_PPM(PPMImage *image);

/**
 * @brief Applies a Gaussian blur filter to a PPM image.
 *
 * @param image The input image to apply the filter to.
 * @return A new PPMImage pointer containing the filtered image.
 */
PPMImage *gaussian_blur_PPM(PPMImage *image);

/**
 * @brief Applies a motion blur filter to a PPM image.
 *
 * @param image The input image to apply the filter to.
 * @return A new PPMImage pointer containing the filtered image.
 */
PPMImage *motion_blur_PPM(PPMImage *image);

/**
 * @brief Applies a sharpen filter to a PPM image.
 *
 * @param image The input image to apply the filter to.
 * @return A new PPMImage pointer containing the filtered image.
 */
PPMImage *sharpen_PPM(PPMImage *image);

/**
 * @brief Applies an extreme sharpen filter to a PPM image.
 *
 * @param image The input image to apply the filter to.
 * @return A new PPMImage pointer containing the filtered image.
 */
PPMImage *extreme_sharpen_PPM(PPMImage *image);

/**
 * @brief Applies an edge detection filter to a PPM image.
 *
 * @param image The input image to apply the filter to.
 * @return A new PPMImage pointer containing the filtered image.
 */
PPMImage *find_edges_PPM(PPMImage *image);

/**
 * @brief Applies an emboss filter to a PPM image.
 *
 * @param image The input image to apply the filter to.
 * @return A new PPMImage pointer containing the filtered image.
 */
PPMImage *emboss_PPM(PPMImage *image);

/**
 * @brief Applies an extreme emboss filter to a PPM image.
 *
 * @param image The input image to apply the filter to.
 * @return A new PPMImage pointer containing the filtered image.
 */
PPMImage *extreme_emboss_PPM(PPMImage *image);

/**
 * @brief Applies a random filter to a PPM image.
 *
 * @param image The input image to apply the filter to.
 * @return A new PPMImage pointer containing the filtered image.
 */
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


