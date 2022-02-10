#include <stdio.h>
#include <math.h>
#include <time.h>
double arr[10000000];
int main() {
    clock_t begin = clock();
    double step = M_2_PI/9999999;
    double sum = 0;
#pragma acc data copyin(arr) copyout(sum)
#pragma acc kernels
    {
    for(int i =0; i < 10000000; i++)
        arr[i] = sin(step * i);
    clock_t end = clock();
    printf("%g\n", (double)((double)(end - begin)/1000));
	
    begin = clock();
    for (int i =0; i < 10000000; i++)
        sum+= arr[i];
    end = clock();
    printf("%g, %g\n",sum ,(double)((double)(end - begin)/1000));
    }
    return 0;
}
