#include "image_filters.hpp"

void changeColorPPM(PPMImage *img) {
    int i;
    if (img) {
        for (i = 0; i < img->x * img->y; i++) {
            img->data[i].red = RGB_COMPONENT_COLOR - img->data[i].red;
            img->data[i].green = RGB_COMPONENT_COLOR - img->data[i].green;
            img->data[i].blue = RGB_COMPONENT_COLOR - img->data[i].blue;
        }
    }
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
    image = readPPM(input_file);

    // changeColorPPM(image);
    writePPM(output_file, image);
    return 0;
}
