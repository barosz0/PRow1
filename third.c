#include <stdio.h>
#include <time.h>
#include <math.h>
#include <string.h>
#include <omp.h>
#include <stdlib.h>


int main(int argc, char* argv[])
{
    clock_t spstart, spstop, ppstart, ppstop;

    int n = 0, m = 100000000;

    double sswtime, sewtime;
    //volatile

    char* isprime = (char*)calloc(m, sizeof(char));

    //memset(isprime, 0, sizeof(isprime));
    isprime[0] = 1;
    isprime[1] = 1;


    sswtime = omp_get_wtime();
    spstart = clock();

    int id;
    omp_set_num_threads(6);
#pragma omp parallel for schedule(static, 1)
    for ( id = 0; id < omp_get_num_threads(); id++)
    {
        int slice = m / omp_get_num_threads();

        int from = id * slice;
        int to = from + slice;
        if (to > m)
            to = m;
        // printf("%d: %d %d\n", id, from, to);
        // int to = m - i*slice;
        // int from = to - slice;

        // if (from < 0)
        //     from = 0;



        for (int i = 0; i < to; i++)
        {
            if (!isprime[i])
            {
                for (int j = i * 2; j < m; j += i)
                {
                    if (j >= from)
                        isprime[j] = 1;
                }
            }

        }

    }


    spstop = clock();
    sewtime = omp_get_wtime();

    int prime_nums = 0;

    for (int i = 0; i < m; i++) {
        if (!isprime[i]) {
            // printf("%d, ", i);
            prime_nums++;
        }
    }
    printf("\n%d \n", prime_nums);

    printf("Czas procesor�w przetwarzania rownoleglego  %f sekund \n", ((double)(spstop - spstart) / CLOCKS_PER_SEC));
    printf("Czas trwania obliczen rownoleglego - wallclock %f sekund \n", sewtime - sswtime);
    return 0;
}