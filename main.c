//OpenMP to implement Histogram

#include <stdio.h>
#include <omp.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

void histogram(int * data, int n, int * bin, int k){
    int bins_no = 800 / k;
    int no_threads = omp_get_max_threads();
    int my_bins[no_threads + 1][1024];
    memset(bin, 0, sizeof(bin));
    memset(my_bins, 0, sizeof(my_bins));
#pragma omp parallel
    {
        int id = omp_get_thread_num();
        int i, j;

#pragma omp for
        for(i = 0 ; i < n ; ++i)
            my_bins[id][data[i] / k] ++;

#pragma omp for
        for(i = 0 ; i < bins_no; ++i)
        {
            for(j = 0 ; j < no_threads; ++j)
            {
                bin[i] += my_bins[j][i];
            }
        }
    }
}

void n_histogram(int * data, int n, int * bins, int k){
    int num_bins = 800 / k;
    int i;
    memset(bins, 0, sizeof(bins));
    for(i=0;i<n;i++)bins[data[i] / k] ++;
}

int main()
{
    int num, count, *array, *bins, i, *bins2;
    long c;
    num = 4 * 10000 * 10000;
    count = 10;
    srand(time(NULL));
    c = clock();
    array = (int *)malloc(sizeof(int) * num);
    bins = (int *)malloc(sizeof(int) * 800 / count);
    bins2 = (int *)malloc(sizeof(int) * 800 / count);
    for(i=0;i<num;++i)
        array[i] = rand() % 800;
    printf("%lf\n", ((double)clock() - c) / CLOCKS_PER_SEC);
    histogram(array, num, bins, count);
    n_histogram(array, num, bins2, count);
    for(i=0;i<800/count;++i)
        if(bins[i] != bins2[i])
        {
            fprintf(stderr, "%d: %d %d WRONG!!\n",i, bins[i], bins2[i]);
            break;
        }
    if(i== 800 / count)
        fprintf(stderr, "OK!\n");

    return 0;
}


