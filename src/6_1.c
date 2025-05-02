#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define THREADS 4

int* array = NULL;
int array_size = 0;
int target = 0;           
int last_index = -1;

pthread_mutex_t mutex;

typedef struct {
    int start;
    int end;
} threadArgs;

void* search(void* arg) {
    threadArgs* args = (threadArgs*)arg;

    for (int i = args->end - 1; i >= args->start; --i) {
        if (array[i] == target) {
            pthread_mutex_lock(&mutex);
            if (i > last_index) {
                last_index = i;
            }
            pthread_mutex_unlock(&mutex);
        }
    }
    pthread_exit(NULL);
}

int main(void) {
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
    free(array);
    if (last_index != -1) {
        fprintf(stdout, "\nLast enter of num %d has been indexed by %d\n", target, last_index);
    } else {
        fprintf(stderr, "\nNumber %d doesn't exist\n", target);
    }

    return 0;
}