#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#define MAX 20
#define NUM_THREADS 10
#define ROWS_PER_THREAD (MAX / NUM_THREADS)

int matA[MAX][MAX];
int matB[MAX][MAX];

int matSumResult[MAX][MAX];
int matDiffResult[MAX][MAX];
int matProductResult[MAX][MAX];

void fillMatrix(int matrix[MAX][MAX]) {
    for(int i = 0; i<MAX; i++) {
        for(int j = 0; j<MAX; j++) {
            matrix[i][j] = rand()%10+1;
        }
    }
}

void printMatrix(int matrix[MAX][MAX]) {
    for(int i = 0; i<MAX; i++) {
        for(int j = 0; j<MAX; j++) {
            printf("%5d", matrix[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

// Each thread handles ROWS_PER_THREAD consecutive rows.
void* computeSum(void* args) {
    int t = *(int*)args;
    free(args);
    int start = t * ROWS_PER_THREAD;
    int end = start + ROWS_PER_THREAD;
    for (int i = start; i < end; i++)
        for (int j = 0; j < MAX; j++)
            matSumResult[i][j] = matA[i][j] + matB[i][j];
    return NULL;
}

void* computeDiff(void* args) {
    int t = *(int*)args;
    free(args);
    int start = t * ROWS_PER_THREAD;
    int end = start + ROWS_PER_THREAD;
    for (int i = start; i < end; i++)
        for (int j = 0; j < MAX; j++)
            matDiffResult[i][j] = matA[i][j] - matB[i][j];
    return NULL;
}

void* computeProduct(void* args) {
    int t = *(int*)args;
    free(args);
    int start = t * ROWS_PER_THREAD;
    int end = start + ROWS_PER_THREAD;
    for (int i = start; i < end; i++)
        for (int j = 0; j < MAX; j++) {
            int sum = 0;
            for (int k = 0; k < MAX; k++)
                sum += matA[i][k] * matB[k][j];
            matProductResult[i][j] = sum;
        }
    return NULL;
}

// Spawns NUM_THREADS threads running fn, then waits for all of them.
static void runThreads(void* (*fn)(void*)) {
    pthread_t threads[NUM_THREADS];
    for (int t = 0; t < NUM_THREADS; t++) {
        int* arg = malloc(sizeof(int));
        *arg = t;
        pthread_create(&threads[t], NULL, fn, arg);
    }
    for (int t = 0; t < NUM_THREADS; t++)
        pthread_join(threads[t], NULL);
}

int main() {
    srand(time(0));  // Do Not Remove. Just ignore and continue below.

    fillMatrix(matA);
    fillMatrix(matB);

    printf("Matrix A:\n");
    printMatrix(matA);
    printf("Matrix B:\n");
    printMatrix(matB);

    runThreads(computeSum);
    runThreads(computeDiff);
    runThreads(computeProduct);

    printf("Results:\n");
    printf("Sum:\n");
    printMatrix(matSumResult);
    printf("Difference:\n");
    printMatrix(matDiffResult);
    printf("Product:\n");
    printMatrix(matProductResult);
    return 0;
}