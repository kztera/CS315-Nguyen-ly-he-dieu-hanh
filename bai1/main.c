#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define MAX 100

int M, K, N;
int A[MAX][MAX], B[MAX][MAX], C[MAX][MAX];

void *multiply(void *arg) {
    int *data = (int *)arg;
    int row = data[0];
    int col = data[1];
    int sum = 0;

    for (int i = 0; i < K; i++) {
        sum += A[row][i] * B[i][col];
    }

    C[row][col] = sum;
    pthread_exit(NULL);
}

int main() {
    FILE *fp;
    fp = fopen("data.dat", "r");

    if (fp == NULL) {
        printf("Khong the mo file\n");
        exit(1);
    }

    fscanf(fp, "%d,%d,%d;", &M, &K, &N);

    for (int i = 0; i < M; i++) {
        for (int j = 0; j < K; j++) {
            fscanf(fp, "%d,", &A[i][j]);
        }
    }

    for (int i = 0; i < K; i++) {
        for (int j = 0; j < N; j++) {
            fscanf(fp, "%d,", &B[i][j]);
        }
    }

    fclose(fp);

    pthread_t tid[MAX][MAX];
    int thread_data[MAX][MAX][2];

    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            thread_data[i][j][0] = i;
            thread_data[i][j][1] = j;
            pthread_create(&tid[i][j], NULL, multiply, (void *)thread_data[i][j]);
        }
    }

    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            pthread_join(tid[i][j], NULL);
        }
    }

    printf("Ma tran A:\n");
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < K; j++) {
            printf("%d ", A[i][j]);
        }

        printf("\n");
    }

    printf("Ma tran B:\n");
    for (int i = 0; i < K; i++) {
        for (int j = 0; j < N; j++) {
            printf("%d ", B[i][j]);
        }

        printf("\n");
    }

    printf("Ma tran C:\n");

    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            printf("%d ", C[i][j]);
        }

        printf("\n");
    }
    return 0;
}