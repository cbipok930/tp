#include <stdio.h>
#include <malloc.h>
#include <math.h>
#define N 5
#define H 1/N


void printSqMtx(double** ptr, int sz){
    for (int i =0; i < sz; i++){
        for (int j = 0; j < sz; j++){
            printf("%g ", ptr[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

int main() {
    double ** u = (double **)malloc(N * sizeof(double*));
    for (int index=0;index<N;++index)
    {
        u[index] = (double *)malloc(N * sizeof(double));
        for (int i = 0; i < N; ++i) {
            u[index][i] = 0;

        }
    }

    double tau = pow(10, 3);
    double a = 1;



    double ** u_plus = (double **)malloc(N * sizeof(double *));
    for (int index=0;index<N;++index)
    {
        u_plus[index] = (double *)malloc(N * sizeof(double ));
        for (int i = 0; i < N; ++i) {
            u_plus[index][i] = 0;

        }
    }


    u[0][0] = 10;
    u[0][N-1] = 20;
    u[N-1][0] = 20;
    u[N-1][N-1] = 30;
    u_plus[0][0] = 10;
    u_plus[0][N-1] = 20;
    u_plus[N-1][0] = 20;
    u_plus[N-1][N-1] = 30;
    printSqMtx(u, N);
    printSqMtx(u_plus, N);
    for (int i = 1; i < N - 1; i++) {
        u[0][i] = u[0][0] + (u[0][N - 1] - u[0][0]) / (N - 1) * i;
        u[i][0] = u[0][0] + (u[N - 1][0] - u[0][0]) / (N - 1) * i;
        u[N - 1][i] = u[N - 1][0] + (u[N - 1][N - 1] - u[N - 1][0]) / (N - 1) * i;
        u[i][N - 1] = u[0][N - 1] + (u[N - 1][N - 1] - u[0][N - 1]) / (N - 1) * i;
    }
    printSqMtx(u, N);
    printSqMtx(u_plus, N);
    for (int i = 1; i < N - 1; i++) {
        u_plus[0][i] = u_plus[0][0] + (u_plus[0][N - 1] - u_plus[0][0]) / (N - 1) * i;
        u_plus[i][0] = u_plus[0][0] + (u_plus[N - 1][0] - u_plus[0][0]) / (N - 1) * i;
        u_plus[N - 1][i] = u_plus[N - 1][0] + (u_plus[N - 1][N - 1] - u_plus[N - 1][0]) / (N - 1) * i;
        u_plus[i][N - 1] = u_plus[0][N - 1] + (u_plus[N - 1][N - 1] - u_plus[0][N - 1]) / (N - 1) * i;
    }
    int num_of_iter = 0;
    FILE *output = fopen("u_plus.txt", "w");
    printSqMtx(u, N);
    printSqMtx(u_plus, N);
    for (int _ = 0; _ < 133777; ++_) {

        double maxdiff;
        for (int i = 1; i < N - 1; i++) {
            maxdiff = u[0][0] - u_plus[0][0];
            for (int j = 1; j < N - 1; j++) {
                maxdiff = fabs(u[0] - u_plus[0]);
                u_plus[i][j] = u[i][j] + a * tau * (u[i - 1][j - 1] - 2 * u[i][j] + u[i + 1][j + 1]) / H / H;
                fprintf(output, "%e ", u_plus[i][j]);
                if (maxdiff < fabs(u[i][j] - u_plus[i][j]))
                {
                    maxdiff = fabs(u[i][j] - u_plus[i][j]);
                }
            }
            fprintf(output, "\n");

        }
        printSqMtx(u, N);
        printSqMtx(u_plus, N);
        fprintf(output, "maxdiff: %e \n", maxdiff);
        if (maxdiff < 1.0e-6)
        {
            printf ("num of iter: %d", num_of_iter);
            break;
        }
        num_of_iter++;
        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < N; ++j) {
                u[i][j] = u_plus[i][j];
            }
        }
    }

    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            fprintf(output, "%e ", u[i][j]);
        }
        fprintf(output, "\n");
    }
    fclose(output);

    return 0;
}