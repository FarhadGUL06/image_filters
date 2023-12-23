#include "mpi.h"

#include "image_filters.hpp"

void read_all_images(PPMImage **images, int start, int stop) {
    for (int i = start; i < stop; i++) {
        char filename[50];
        sprintf(filename, "images/ppm_before/image%d.ppm", i);
        images[i] = read_PPM(filename);
    }
}

void write_all_images(PPMImage **images, int start, int stop) {
    for (int i = start; i < stop; i++) {
        char filename[50];
        sprintf(filename, "images/ppm_after/image%d.ppm", i);
        write_PPM(filename, images[i]);
    }
}

void free_all_images(PPMImage **images, int start, int stop,
                     PPMImage *(**to_apply)(PPMImage *)) {
    for (int i = start; i < stop; i++) {
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

void apply_all_filters(PPMImage **images, int start, int stop,
                       PPMImage *(**to_apply)(PPMImage *),
                       int number_of_filters) {
    for (int i = start; i < stop; i++) {
        for (int j = 0; j < number_of_filters; j++) {
            images[i] = to_apply[j](images[i]);
        }
    }
}

int main(int argc, char *argv[]) {
    int rank, size;
    // argv[1] - how many images to take
    if (argc < 2) {
        printf("Usage: ./mpi <number_of_images>\n");
        return 1;
    }
    const int number_of_images = atoi(argv[1]);
    if (number_of_images < 0 || number_of_images > 50) {
        printf("Invalid number of images\n");
        return 1;
    }

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);  // process id
    MPI_Comm_size(MPI_COMM_WORLD, &size);  // number of processes

    int number_of_images_per_process = number_of_images / size;

    PPMImage **images;
    images = (PPMImage **)malloc(sizeof(PPMImage *) * number_of_images);
    read_all_images(images, 0, number_of_images);

    int number_of_filters;
    PPMImage *(**to_apply)(PPMImage *);

    if (rank == 0) {
        
        to_apply = read_all_filters(&number_of_filters);
        // Send with MPI_Bcast
        MPI_Bcast(&number_of_filters, 1, MPI_INT, 0, MPI_COMM_WORLD);
        MPI_Bcast(to_apply, number_of_filters, MPI_UNSIGNED_LONG, 0,
                  MPI_COMM_WORLD);

        if (number_of_images % size != 0) {
            number_of_images_per_process = number_of_images % size;
        }
    }

    printf("Images in current rank %d has %d images\n", rank,
           number_of_images_per_process);

    //apply_all_filters(images, 0, number_of_images_per_process, to_apply,
    //                  number_of_filters);

    write_all_images(images, 0, number_of_images_per_process);
    // free_all_images(images, start, stop, to_apply);
    MPI_Finalize();
    return 0;
}
