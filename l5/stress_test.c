#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>

#define NUM_THREADS 16
#define FIB_LIMIT 100000000000
#define NUM_ITERATIONS 100000000000

uint64_t fibonacci(uint64_t n) {
    if (n <= 1) return n;
    uint64_t a = 0, b = 1, c;
    for (uint64_t i = 2; i <= n; ++i) {
        c = a + b;
        a = b;
        b = c;
    }
    return b;
}

void* perform_work(void* arg) {
    int thread_num = *((int*)arg);
    uint64_t fib_result;
    for (int i = 0; i < NUM_ITERATIONS; ++i) {
        fib_result = fibonacci(FIB_LIMIT);
    }
    printf("Thread %d finished\n", thread_num);
    return NULL;
}

int main() {
    pthread_t threads[NUM_THREADS];
    int thread_nums[NUM_THREADS];

    for (int i = 0; i < NUM_THREADS; ++i) {
        thread_nums[i] = i;
        int err = pthread_create(&threads[i], NULL, perform_work, &thread_nums[i]);
        if (err) {
            printf("Error creating thread %d\n", i);
            return 1;
        }
    }

    for (int i = 0; i < NUM_THREADS; ++i) {
        pthread_join(threads[i], NULL);
    }

    printf("All threads finished\n");
    return 0;
}
