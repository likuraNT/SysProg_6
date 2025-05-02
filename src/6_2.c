#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define THREADS 4

int* array = NULL;
int array_size = 0;
int target = 0;

int* result_array = NULL;
int result_count = 0;
int result_size = 0;

pthread_mutex_t mutex;

typedef struct {
    int start;
    int end;
} threadArgs;

void* search(void* arg) {
    threadArgs* args = (threadArgs*)arg;

    for (int i = args->start; i < args->end; ++i) {
        if (array[i] == target) {
            pthread_mutex_lock(&mutex);

            if (result_count >= result_size) {
                result_size = (result_size == 0) ? 10 : result_size * 2;
                result_array = realloc(result_array, result_size * sizeof(int));
            }
            result_array[result_count++] = i;
            pthread_mutex_unlock(&mutex);
        }
    }
    pthread_exit(NULL);
}

int qsort_pton(const void* a, const void* b) {
    return (*(int*)b - *(int*)a);
}

int main() {
    printf("Enter a size of array: ");
    scanf("%d", &array_size);
    array = (int*)malloc(array_size * sizeof(int));

    printf("Enter the elements of an array: \n");
    for (int i = 0; i < array_size; ++i) {
        scanf("%d", &array[i]);
    }

    fprintf(stdout, "Enter your searching number: ");
    scanf("%d", &target);

    pthread_t threads[THREADS];
    threadArgs thread_args[THREADS];

    pthread_mutex_init(&mutex, NULL);

    int chunk_size = array_size / THREADS;

    for (int i = 0; i < THREADS; ++i) {
        thread_args[i].start = i * chunk_size;
        thread_args[i].end = (i == THREADS - 1) ? array_size : (i + 1) * chunk_size;
        pthread_create(&threads[i], NULL, search, &thread_args[i]);
    }

    for (int i = 0; i < THREADS; ++i) {
        pthread_join(threads[i], NULL);
    }
    pthread_mutex_destroy(&mutex);

    qsort(result_array, result_count, sizeof(int), qsort_pton);

    if (result_count > 0) {
        fprintf(stdout, "Finded %d enterances. Sorted indexes:\n", result_count);
        for (int i = 0; i < result_count; ++i) {
            fprintf(stdout, "%d\n", result_array[i]);
        }
    } else {
        fprintf(stderr, "\nNumber %d doesn't exist\n", target);
    }
    free(array);
    free(result_array);
    return 0;
}