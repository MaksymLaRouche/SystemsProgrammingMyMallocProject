#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include "mymalloc.h"

// Task 1: Allocate sizes 8..1024, then free in reverse order
void task1(void) {
    int sizes[] = {8, 16, 32, 64, 128, 512, 1024};
    void *ptrs[7];

    for (int i = 0; i < 7; i++) {
        ptrs[i] = malloc(sizes[i]);
    }
    for (int i = 6; i >= 0; i--) {
        if (ptrs[i] != NULL) free(ptrs[i]);
    }
}

// Task 2: Allocate 120 small objects, then free in order allocated
void task2(void) {
    void *ptrs[120];

    for (int i = 0; i < 120; i++) {
        ptrs[i] = malloc(1);
    }
    for (int i = 0; i < 120; i++) {
        free(ptrs[i]);
    }
}

// Task 3: Randomly allocate or free until 120 allocations occur
void task3(void) {
    void *ptrs[120] = {NULL};
    int alloc_count = 0;
    int active_count = 0;

    while (alloc_count < 120) {
        int action = rand() % 2; // 0 = allocate, 1 = free

        if (action == 0 || active_count == 0) {
            for (int i = 0; i < 120; i++) {
                if (ptrs[i] == NULL) {
                    ptrs[i] = malloc(1);
                    alloc_count++;
                    active_count++;
                    break;
                }
            }
        } else {
            int idx = rand() % 120;
            while (ptrs[idx] == NULL) {
                idx = (idx + 1) % 120;
            }
            free(ptrs[idx]);
            ptrs[idx] = NULL;
            active_count--;
        }
    }

    // Free any remaining pointers
    for (int i = 0; i < 120; i++) {
        if (ptrs[i] != NULL) {
            free(ptrs[i]);
            ptrs[i] = NULL;
        }
    }
}

// Task 4: Alternating free test (evens first, then odds)
void task4(void) {
    void *ptrs[50];

    for (int i = 0; i < 50; i++) {
        ptrs[i] = malloc(16);
    }
    for (int i = 0; i < 50; i += 2) {
        free(ptrs[i]);
    }
    for (int i = 1; i < 50; i += 2) {
        free(ptrs[i]);
    }
}

// Task 5: Fill heap until full, then free all allocated chunks
void task5(void) {
    void *ptrs[100];
    int count = 0;

    for (int i = 0; i < 100; i++) {
        ptrs[i] = malloc(16);
        if (ptrs[i] != NULL) {
            count++;
        } else {
            break;
        }
    }
    for (int i = 0; i < count; i++) {
        free(ptrs[i]);
    }
}

int main(void) {
    struct timeval start, end;
    long time_us;

    // Time Task 1
    gettimeofday(&start, NULL);
    for (int i = 0; i < 50; i++) task1();
    gettimeofday(&end, NULL);
    time_us = (long)((end.tv_sec - start.tv_sec) * 1000000 + (end.tv_usec - start.tv_usec));
    printf("Task 1 average time: %ld microseconds\n", time_us / 50);

    // Time Task 2
    gettimeofday(&start, NULL);
    for (int i = 0; i < 50; i++) task2();
    gettimeofday(&end, NULL);
    time_us = (long)((end.tv_sec - start.tv_sec) * 1000000 + (end.tv_usec - start.tv_usec));
    printf("Task 2 average time: %ld microseconds\n", time_us / 50);

    // Time Task 3
    gettimeofday(&start, NULL);
    for (int i = 0; i < 50; i++) task3();
    gettimeofday(&end, NULL);
    time_us = (long)((end.tv_sec - start.tv_sec) * 1000000 + (end.tv_usec - start.tv_usec));
    printf("Task 3 average time: %ld microseconds\n", time_us / 50);

    // Time Task 4
    gettimeofday(&start, NULL);
    for (int i = 0; i < 50; i++) task4();
    gettimeofday(&end, NULL);
    time_us = (long)((end.tv_sec - start.tv_sec) * 1000000 + (end.tv_usec - start.tv_usec));
    printf("Task 4 average time: %ld microseconds\n", time_us / 50);

    // Time Task 5
    gettimeofday(&start, NULL);
    for (int i = 0; i < 50; i++) task5();
    gettimeofday(&end, NULL);
    time_us = (long)((end.tv_sec - start.tv_sec) * 1000000 + (end.tv_usec - start.tv_usec));
    printf("Task 5 average time: %ld microseconds\n", time_us / 50);

    return 0;
}