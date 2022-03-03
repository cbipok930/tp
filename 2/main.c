#include <stdio.h>
#include<math.h>
#include <stdlib.h>
#include <malloc.h>
//#define  N 1024

int main(int argc, char* argv[]) {
    int N;
    int itMAX;
    double limMAX;
    if (argc < 4){
        printf("too few arguments");
        exit(1);
    }
    else
    {
        limMAX = (double)atoi(argv[1]);
        if ((int)limMAX == 0){
            printf("incorrect first param");
            exit(1);
        }
        N = atoi(argv[2]);
        if (N == 0){
            printf("incorrect second param");
            exit(1);
        }
        itMAX = atoi(argv[3]);
        if (itMAX == 0){
            printf("incorrect third param");
            exit(1);
        }
    }
    double **u = (double **) calloc(N, sizeof(double *)); // first

    for (int i = 0; i < N; i++) {
        u[i] = (double *) calloc(N, sizeof(double));
    }

    double **u_plus = (double **) calloc(N, sizeof(double *));  // second

    for (int j = 0; j < N; j++) {
        u_plus[j] = (double *) calloc(N, sizeof(double));
    }

    u[0][0] = 10.0;
    u[N - 1][0] = 20.0;
    u[0][N - 1] = 20.0;
    u[N - 1][N - 1] = 30.0;

    u_plus[0][0] = 10.0;
    u_plus[N - 1][0] = 20.0;
    u_plus[0][N - 1] = 20.0;
    u_plus[N - 1][N - 1] = 30.0;

    double step = 10.0 / (N - 1);
    int it = 0;
    double a = 0.25;
    double lim = 1;

#pragma acc kernels
    for (int k = 1; k < N - 1; k++) {
        u[k][0] = 10 + step * k;
        u[0][k] = 10 + step * k;
        u[k][N - 1] = 20 + step * k;
        u[N - 1][k] = 20 + step * k;
        u_plus[k][0] = 10 + step * k;
        u_plus[0][k] = 10 + step * k;
        u_plus[k][N - 1] = 20 + step * k;
        u_plus[N - 1][k] = 20 + step * k;

    }

#pragma acc data copy(u[0:N][0:N], lim) create (u_plus[0:N][0:N])
    {
        while (lim > pow(10, -limMAX) && it < pow(10, itMAX)) {

            it++;
            if(it % 100 == 0 ){
#pragma acc kernels async
                lim = 0;
            }
#pragma acc data present(u, u_plus)
#pragma acc parallel num_gangs(128) async
            {
                if(it % 100 == 0){

#pragma acc loop collapse(2) independent reduction(max:lim)
                    for (int i = 1; i < N - 1; i++) {

                        for (int j = 1; j < N - 1; j++) {

                            u_plus[i][j] = a * (u[i + 1][j] + u[i - 1][j] + u[i][j - 1] + u[i][j + 1]);
                            lim = fmax(lim, u_plus[i][j] - u[i][j]);
                        }
                    }
                }
                else{
#pragma acc loop collapse(2)

                    for (int i = 1; i < N - 1; i++) {
                        for (int j = 1; j < N - 1; j++) {

                            u_plus[i][j] = a * (u[i + 1][j] + u[i - 1][j] + u[i][j - 1] + u[i][j + 1]);
                        }
                    }

                }
            }
#pragma acc parallel loop independent collapse(2) async
            for (int i = 1; i < N - 1; i++) {
                for (int j = 1; j < N - 1; j++) {

                    u[i][j] = u_plus[i][j];
                }
            }

            if(it % 100 == 0 ){
#pragma acc wait
#pragma acc update self(lim)
                printf("%d %e\n", it, lim);
            }


        }
    }


    return 0;
}