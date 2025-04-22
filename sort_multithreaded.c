#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

// Global arrays
#define SIZE 10
int array[SIZE] = {7, 2, 9, 4, 5, 1, 3, 8, 6, 10}; // Input array
int sorted_array[SIZE]; // Final sorted array

// Structure to pass arguments to sorting threads
typedef struct {
    int start;
    int end;
} SortArgs;

// Bubble Sort function for a portion of the array
void bubble_sort(int arr[], int start, int end) {
    for (int i = start; i < end; i++) {
        for (int j = start; j < end - (i - start) - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

// Thread function to sort a portion of the array
void* sort_thread(void* arg) {
    SortArgs* args = (SortArgs*)arg;
    bubble_sort(array, args->start, args->end);
    return NULL;
}

// Merge function to combine two sorted halves
void merge(int arr[], int left, int mid, int right, int result[]) {
    int i = left, j = mid, k = 0;
    while (i < mid && j < right) {
        if (arr[i] <= arr[j]) {
            result[k++] = arr[i++];
        } else {
            result[k++] = arr[j++];
        }
    }
    while (i < mid) {
        result[k++] = arr[i++];
    }
    while (j < right) {
        result[k++] = arr[j++];
    }
}

// Thread function to merge sorted halves (renamed to avoid conflict)
void* merge_thread_func(void* arg) {
    merge(array, 0, SIZE / 2, SIZE, sorted_array);
    return NULL;
}

int main() {
    pthread_t sort_threads[2], merge_thread;
    SortArgs args[2];

    // Set arguments for sorting threads
    args[0].start = 0;
    args[0].end = SIZE / 2; // First half
    args[1].start = SIZE / 2;
    args[1].end = SIZE; // Second half

    // Create sorting threads
    pthread_create(&sort_threads[0], NULL, sort_thread, &args[0]);
    pthread_create(&sort_threads[1], NULL, sort_thread, &args[1]);

    // Wait for sorting threads to finish
    pthread_join(sort_threads[0], NULL);
    pthread_join(sort_threads[1], NULL);

    // Create merge thread
    pthread_create(&merge_thread, NULL, merge_thread_func, NULL);

    // Wait for merge thread to finish
    pthread_join(merge_thread, NULL);

    // Print final sorted array
    printf("Sorted array: ");
    for (int i = 0; i < SIZE; i++) {
        printf("%d ", sorted_array[i]);
    }
    printf("\n");

    return 0;
}