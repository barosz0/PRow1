#include <stdio.h>
#include <time.h>
#include <math.h>
#include <string.h>
#include <omp.h>
#include <stdlib.h>

# define Th_num 6


void printPrimes(char* primes, int max, int min) {
    int primeNums = 0;
    int count = 0;
    for (int i = 0; i <= max - min; i++) {
        if (primes[i] == 1) {
            primeNums++;
            count++;
            if (count == 10) {
                printf("%d\n", i + min);
                count = 0;
            }
            else
                printf("%d ", i + min);
        }
    }
    printf("\nLiczb pierwszych: %d\n", primeNums);
}

void printPrimesNum(char* primes, int max, int min) {
    int primeNums = 0;
    for (int i = 0; i <= max - min; i++) {
        if (primes[i] == 1) primeNums++;
    }
    printf("Liczb pierwszych: %d\n", primeNums);
}

void sito_sekwencyjnie(int n, int m)
{
    clock_t spstart, spstop, ppstart, ppstop;

    //int n = 0;
    //int m = 100000000;

    double sswtime, sewtime;
    //volatile

    char* isprime = (char*)calloc(m, sizeof(char));

    //memset(tab, 0, sizeof(tab));
    isprime[0] = 1;
    isprime[1] = 1;

    //SEKWENCYJNIE
    sswtime = omp_get_wtime();
    spstart = clock();

    for (int i = 0; i < m; i++)
    {

        if (isprime[i])
        {
            continue;
        }

        int j = 2;
        int num = i;
        while (num * j < m)
        {
            //printf("%d kasuje %d \n",i, num *j);
            isprime[num * j] = 1;
            j++;
        }
    }

    spstop = clock();
    sewtime = omp_get_wtime();

    int prime_nums = 0;

    for (int i = n; i < m; i++) {
        if (isprime[i] == 0)
            //printf("%d, ",i);
            prime_nums++;
    }
    printf("%d \n", prime_nums);

    //printPrimesNum(primes, m, n);

    printf("Czas procesorow przetwarzania sekwencyjnego  %f sekund \n", ((double)(spstop - spstart) / CLOCKS_PER_SEC));
    printf("Czas trwania obliczen sekwencyjnych - wallclock %f sekund \n", sewtime - sswtime);
}

void sito_rownolegle(int n, int m)
{
	clock_t spstart, spstop,ppstart,ppstop;
	

	double sswtime, sewtime;
    //volatile

    
    char* isprime = (char*)calloc(m, sizeof(char));

    isprime[0]=1;
    isprime[1]=1;


	sswtime = omp_get_wtime();
	spstart = clock();

    int i =0;
    omp_set_num_threads(Th_num);
    #pragma omp parallel for schedule(static)
    for (i =1; i<m;i++)
    {
        //printf("%d ma %d\n",omp_get_thread_num(), i);
        if(!isprime[i])
            for(int j = i*2; j<m; j+=i)
                isprime[j]=1;
    }


	spstop = clock();
    sewtime = omp_get_wtime();
    
    int prime_nums = 0;

    for(int i = n;i<m;i++){
        if(!isprime[i])
            prime_nums++;
    }
    printf("%d \n",prime_nums);

	printf("Czas procesorów przetwarzania rownoleglego  %f sekund \n", ((double)(spstop - spstart)/CLOCKS_PER_SEC));
	printf("Czas trwania obliczen rownoleglego - wallclock %f sekund \n",  sewtime-sswtime);	
}

void sito_rownolegle_blokowo(int n, int m)
{
    clock_t spstart, spstop, ppstart, ppstop;

    double sswtime, sewtime;
    //volatile

    char* isprime = (char*)calloc(m, sizeof(char));

    //memset(isprime, 0, sizeof(isprime));
    isprime[0] = 1;
    isprime[1] = 1;


    sswtime = omp_get_wtime();
    spstart = clock();

    int id;
    omp_set_num_threads(Th_num);
    #pragma omp parallel for schedule(static)
    for ( id = 0; id < omp_get_num_threads(); id++)
    {
        int slice = m / omp_get_num_threads();

        int from = id * slice;
        int to = from + slice;
        if (to > m)
            to = m;
        //printf("%d: %d %d\n", id, from, to);
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
            //printf("%d, ", i);
            prime_nums++;
        }
    }
    printf("%d \n", prime_nums);

    printf("Czas procesor�w przetwarzania rownoleglego  %f sekund \n", ((double)(spstop - spstart) / CLOCKS_PER_SEC));
    printf("Czas trwania obliczen rownoleglego - wallclock %f sekund \n", sewtime - sswtime);
}

void dzielenie_sekwecyjnie(int min, int max) {
    clock_t spstart, spstop;
    double sswtime, sewtime;

    //1 - prime, 2 - not a prime
    char* primes;

    //SEKWENCYJNIE
    primes = (char*)calloc(max - min, sizeof(char));
    sswtime = omp_get_wtime();
    spstart = clock();

    for (int i = 0; i <= max - min; i++) {
        primes[i] = 1;
        for (int j = 2; j <= (int)ceil(sqrt(i + min)); j++) {
            if (((i + min) % j == 0) && (i + min != 2)) {
                primes[i] = 2;
                break;
            }
        }
    }

    spstop = clock();
    sewtime = omp_get_wtime();

    printPrimesNum(primes, max, min);

    free(primes);

    printf("Czas procesor�w przetwarzania sekwencyjnego  %f sekund \n", ((double)(spstop - spstart) / CLOCKS_PER_SEC));
    printf("Czas trwania obliczen segwencyjnego - wallclock %f sekund \n", sewtime - sswtime);
}

void dzielenie_rownolegle(int min, int max) {
    clock_t ppstart, ppstop;
    double pswtime, pewtime;

    //1 - prime, 2 - not a prime
    char* primes;

    //RÓWNOLEGLE
    primes = (char*)calloc(max - min, sizeof(int));

    pswtime = omp_get_wtime();
    ppstart = clock();
    int minp, maxp;
    omp_set_num_threads(Th_num);
    #pragma omp parallel private(minp, maxp)
    {
        minp = min;
        maxp = max;
        int i;
        #pragma omp for
        for (i = 0; i <= maxp - minp; i++) {
            primes[i] = 1;
            for (int j = 2; j <= (int)ceil(sqrt(i + minp)); j++) {
                if (((i + minp) % j == 0) && (i + minp != 2)) {
                    primes[i] = 2;
                    break;
                }
            }
        }
    }

    ppstop = clock();
    pewtime = omp_get_wtime();

    printPrimesNum(primes, max, min);

    free(primes);

    printf("Czas procesorów przetwarzania rownoleglego  %f sekund \n", ((double)(ppstop - ppstart) / CLOCKS_PER_SEC));
    printf("Czas trwania obliczen rownoleglego - wallclock %f sekund \n", pewtime - pswtime);
}

int main(int argc, char* argv[])
{
    int n = 2;
    int m = 10000;


    sito_sekwencyjnie(n,m);
    //sito_rownolegle(n,m);
    //sito_rownolegle_blokowo(n,m);
    //dzielenie_sekwecyjnie(n, m);
    //dzielenie_rownolegle(n, m);

    return 0;
}

//
// gcc one.c -lm -fopenmp