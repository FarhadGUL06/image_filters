#include "image_filters.hpp"

void negative_PPM(PPMImage *img) {
    int i;
    if (img) {
        for (i = 0; i < img->height * img->width; i++) {
            img->data[i].red = RGB_COMPONENT_COLOR - img->data[i].red;
            img->data[i].green = RGB_COMPONENT_COLOR - img->data[i].green;
            img->data[i].blue = RGB_COMPONENT_COLOR - img->data[i].blue;
        }
    }
}

void blur_PPM(PPMImage *img, int boxBlur) {
    int i;
    int height = img->width;
    int width = img->height;
    int red, green, blue;
    int counter;

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            red = 0;
            green = 0;
            blue = 0;
            counter = 0;

            for (int k = i - boxBlur; k <= i + boxBlur; ++k) {
                if (k < 0) {
                    k = 0;
                }
                if (k >= height) {
                    break;
                }
                for (int l = j - boxBlur; l <= j + boxBlur; ++l) {
                    if (l < 0) {
                        l = 0;
                    }
                    if (l >= width) {
                        break;
                    }
                    red += img->data[k * width + l].red;
                    green += img->data[k * width + l].green;
                    blue += img->data[k * width + l].blue;
                    counter++;
                }
            }

            img->data[i * width + j].red = red / counter;
            img->data[i * width + j].green = green / counter;
            img->data[i * width + j].blue = blue / counter;
        }
    }
}

void motion_blur_horizontaly_PPM(PPMImage *img, int boxBlur) {
    int i;
    int height = img->width;
    int width = img->height;
    int red, green, blue;
    int counter;

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            red = 0;
            green = 0;
            blue = 0;
            counter = 0;
            for (int l = j - boxBlur; l <= j + boxBlur; ++l) {
                if (l < 0) {
                    l = 0;
                }
                if (l >= width) {
                    break;
                }
                red += img->data[i * width + l].red;
                green += img->data[i * width + l].green;
                blue += img->data[i * width + l].blue;
                counter++;
            }

            img->data[i * width + j].red = red / counter;
            img->data[i * width + j].green = green / counter;
            img->data[i * width + j].blue = blue / counter;
        }
    }
}

void motion_blur_verticaly_PPM(PPMImage *img, int boxBlur) {
    int i;
    int height = img->width;
    int width = img->height;
    int red, green, blue;
    int counter;

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; ++j) {
            red = 0;
            green = 0;
            blue = 0;
            counter = 0;

            for (int k = i - boxBlur; k <= i + boxBlur; ++k) {
                if (k < 0) {
                    k = 0;
                }
                if (k >= height) {
                    break;
                }

                red += img->data[k * width + j].red;
                green += img->data[k * width + j].green;
                blue += img->data[k * width + j].blue;
                counter++;
            }

            img->data[i * width + j].red = red / counter;
            img->data[i * width + j].green = green / counter;
            img->data[i * width + j].blue = blue / counter;
        }
    }
}

void greyscale_PPM(PPMImage *img) {
    int i;
    if (img) {
        for (i = 0; i < img->height * img->width; i++) {
            int avg =
                (img->data[i].red + img->data[i].green + img->data[i].blue) / 3;
            img->data[i].red = avg;
            img->data[i].green = avg;
            img->data[i].blue = avg;
        }
    }
}

#define filterWidth 3
#define filterHeight 3

void mirror_verticaly_PPM(PPMImage *img) {
    int i;
    int height = img->width;
    int width = img->height;
    int red, green, blue;

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width / 2; j++) {
            red = img->data[i * width + j].red;
            green = img->data[i * width + j].green;
            blue = img->data[i * width + j].blue;

            img->data[i * width + j].red =
                img->data[i * width + width - j - 1].red;
            img->data[i * width + j].green =
                img->data[i * width + width - j - 1].green;
            img->data[i * width + j].blue =
                img->data[i * width + width - j - 1].blue;

            img->data[i * width + width - j - 1].red = red;
            img->data[i * width + width - j - 1].green = green;
            img->data[i * width + width - j - 1].blue = blue;
        }
    }
}

void mirror_horizontal_PPM(PPMImage *img) {
    int i;
    int height = img->width;
    int width = img->height;
    int red, green, blue;

    for (int i = 0; i < height / 2; i++) {
        for (int j = 0; j < width; j++) {
            red = img->data[i * width + j].red;
            green = img->data[i * width + j].green;
            blue = img->data[i * width + j].blue;

            img->data[i * width + j].red =
                img->data[(height - i - 1) * width + j].red;
            img->data[i * width + j].green =
                img->data[(height - i - 1) * width + j].green;
            img->data[i * width + j].blue =
                img->data[(height - i - 1) * width + j].blue;

            img->data[(height - i - 1) * width + j].red = red;
            img->data[(height - i - 1) * width + j].green = green;
            img->data[(height - i - 1) * width + j].blue = blue;
        }
    }
}

void sepia_PPM(PPMImage *img) {
    int i;
    if (img) {
        for (i = 0; i < img->height * img->width; i++) {
            int red = img->data[i].red;
            int green = img->data[i].green;
            int blue = img->data[i].blue;

            img->data[i].red =
                min((int)(0.393 * red + 0.769 * green + 0.189 * blue), 255);
            img->data[i].green =
                min((int)(0.349 * red + 0.686 * green + 0.168 * blue), 255);
            img->data[i].blue =
                min((int)(0.272 * red + 0.534 * green + 0.131 * blue), 255);
        }
    }
}
/*
void emboss_PPM(PPMImage *img) {
    PPMImage *emb_img = (PPMImage *)malloc(sizeof(PPMImage));
    Pixel diff;
    Pixel upleft;
    Pixel curr;
    char maxDiff, tmp = 0;
    unsigned char v;
    int x, y;
    for (y = 1; y < img->height; y++) {
        for (x = 1; x < img->width; x++) {
            if (maxDiff > tmp)
                tmp = maxDiff;
            else
                maxDiff = tmp;

            upleft = getPixel(img, x - 1, y - 1);
            curr = getPixel(img, x, y);
            diff.r = curr.r - upleft.r;
            diff.g = curr.g - upleft.g;
            diff.b = curr.b - upleft.b;
            if ((diff.r == diff.g && diff.r > diff.b) ||
                (diff.r == diff.b && diff.r > diff.g))
                maxDiff = diff.r;
            else
                maxDiff = max(diff.r, max(diff.g, diff.b));

            v = 128 + maxDiff;
            if (v < 0) v = 0;
            if (v > 255) v = 255;

            Pixel val2 = {v, v, v};
            setPixel(x, y, &emb_img, val2);
        }
    }
    return emb_img;
}*/

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

    // mirrored_verticaly_PPM(image);
    // blur_PPM(image, 1);
    // greyscale_PPM(image);
    // mirror_horizontal_PPM(image);
    // mirror_horizontal_PPM(image);
    // mirror_verticaly_PPM(image);
    // motion_blur_verticaly_PPM(image, 50);
    motion_blur_horizontaly_PPM(image, 50);

    // changeColorPPM(image);
    writePPM(output_file, image);
    return 0;
}
