#include <pthread.h>

#include "image_filters.hpp"

int nr_threads;

// Struct for payload
struct thread_payload {
    size_t thread_id;
    PPMImage **images;
    PPMImage *(**to_apply)(PPMImage *);
    int number_of_filters;
    int start;
    int end;
};

void *thread_image(void *arg) {
    thread_payload *info = (thread_payload *)arg;
    PPMImage **images = info->images;
    size_t thread_id = info->thread_id;
    PPMImage *(**to_apply)(PPMImage *) = info->to_apply;
    int start = info->start;
    int end = info->end;
    int number_of_filters = info->number_of_filters;

    for (int i = start; i < end; i++) {
        for (int j = 0; j < number_of_filters; j++) {
            images[i] = to_apply[j](images[i]);
        }
    }

    for (int i = start; i < end; i++) {
        char filename[50];
        sprintf(filename, "images/ppm_after/image%d.ppm", i);
        write_PPM(filename, images[i]);
    }

    pthread_exit(NULL);
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
    pthread_t tid[nr_threads];
    thread_payload info[nr_threads];

    for (size_t i = 0; i < nr_threads; i++) {
        info[i].thread_id = i;
        info[i].images = images;
        info[i].to_apply = to_apply;
        info[i].number_of_filters = number_of_filters;
        info[i].start = i * number_of_images / nr_threads;
        info[i].end = (i + 1) * number_of_images / nr_threads;
        pthread_create(&tid[i], NULL, thread_image, (void *)&info[i]);
    }

    for (size_t i = 0; i < nr_threads; i++) {
        pthread_join(tid[i], NULL);
    }
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Usage: ./pthread <number_of_images> <num_threads>\n");
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

    nr_threads = atoi(argv[2]);
    apply_all_filters(images, number_of_images, to_apply, number_of_filters);

    // write_all_images(images, number_of_images);
    free_all_images(images, number_of_images, to_apply);
    return 0;
}
