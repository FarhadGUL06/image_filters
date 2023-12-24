#include "image_filters.hpp"

void read_all_images(PPMImage **images, int number_of_images) {
    for (int i = 0; i < number_of_images; i++) {
        char filename[50];
        sprintf(filename, "images/ppm_before/image%d.ppm", i);
        images[i] = read_PPM(filename);
    }
}

void write_all_images(PPMImage **images, int number_of_images) {
    int i;
#pragma omp parallel for private(i) shared(images)
    for (i = 0; i < number_of_images; i++) {
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
    int i;
#pragma omp parallel for private(i) shared(images, to_apply)
    for (i = 0; i < number_of_images; i++) {
        int j;
        // #pragma omp parallel for private(j) shared(images, to_apply)
        for (j = 0; j < number_of_filters; j++) {
            images[i] = to_apply[j](images[i]);
        }
    }
}

int main(int argc, char *argv[]) {
    // argv[1] - how many images to take
    if (argc < 2) {
        printf("Usage: ./serial <number_of_images>\n");
        return 1;
    }
    const int number_of_images = atoi(argv[1]);
    if (number_of_images <= 0 || number_of_images > 50) {
        printf("Invalid number of images\n");
        return 1;
    }

    PPMImage **images =
        (PPMImage **)malloc(sizeof(PPMImage *) * number_of_images);
    read_all_images(images, number_of_images);

    int number_of_filters;
    PPMImage *(**to_apply)(PPMImage *) = read_all_filters(&number_of_filters);

    apply_all_filters(images, number_of_images, to_apply, number_of_filters);

    write_all_images(images, number_of_images);
    free_all_images(images, number_of_images, to_apply);
    return 0;
}
