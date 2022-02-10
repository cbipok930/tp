#include <stdio.h>
#include <math.h>
#include <time.h>
float arr[10000000];
int main() {
    clock_t begin = clock();
    float step = M_2_PI/9999999;
    float sum = 0;
#pragma acc data copyin(arr) copyout(sum)
#pragma acc kernels
    {
    for(int i =0; i < 10000000; i++)
        arr[i] = sin(step * i);
    clock_t end = clock();
    printf("%f\n", (float)((float)(end - begin)/1000));
	
    begin = clock();
    for (int i =0; i < 10000000; i++)
        sum+= arr[i];
    end = clock();
    printf("%f, %f\n",sum ,(float)((float)(end - begin)/1000));
    }
    return 0;
}
