//Bartosz Paliński 145224 L1
//Patryk Chmielecki 145190 L1

#include <stdio.h>
#include <time.h>
#include <math.h>
#include <string.h>
#include <omp.h>
#include <stdlib.h>
#include <conio.h>

# define Th_num 12

void printPrimes(char* primes, long long max, long long min) {
    long long primeNums = 0;
    int count = 0;
    for (long long i = 0; i < max - min; i++) {
        if (primes[i] == 1) {
            primeNums++;
            count++;
            if (count == 10) {
                printf("%lld\n", i + min);
                count = 0;
            }
            else
                printf("%lld ", i + min);
        }
    }
    printf("\nLiczb pierwszych: %lld\n", primeNums);
}

void printPrimesNum(char* primes, long long max, long long min) {
    long long primeNums = 0;
    for (long long i = 0; i < max - min; i++) {
        if (primes[i] == 1) primeNums++;
    }
    printf("Liczb pierwszych: %lld\n", primeNums);
}

void printNumsSito(char* primes, long long min, long long max)
{
    long long primeNums = 0;
    int count = 0;
    for (long long i = 0; i < max - min; i++) {
        if (primes[i] !=1) {
            primeNums++;
            count++;
            if (count == 10) {
                printf("%lld\n", i + min);
                count = 0;
            }
            else
                printf("%lld ", i + min);
        }
    }
    printf("\nLiczb pierwszych: %lld\n", primeNums);
}

void sito_sekwencyjnie(long long n, long long m)
{
    clock_t spstart, spstop, ppstart, ppstop;


    double sswtime, sewtime;


    char* isprime = (char*)calloc(m, sizeof(char));

    isprime[0] = 1;
    isprime[1] = 1;

    //SEKWENCYJNIE
    sswtime = omp_get_wtime();
    spstart = clock();

    long long to = sqrt(m);

    for (long long i = 0; i <= to; i++)
    {

        if (!isprime[i])
        {


            long long j = 2;
            long long num = i;
            while (num * j < m)
            {
                //printf("%d kasuje %d \n",i, num *j);
                isprime[num * j] = 1;
                j++;
            }
        }
    }

    spstop = clock();
    sewtime = omp_get_wtime();

    long long prime_nums = 0;

    for (long long i = n; i < m; i++) {
        if (isprime[i] == 0)
            prime_nums++;
    }
    printf("%lld \n", prime_nums);



    printf("Czas procesorow przetwarzania sekwencyjnego  %f sekund \n", ((double)(spstop - spstart) / CLOCKS_PER_SEC));
    printf("Czas trwania obliczen sekwencyjnych - wallclock %f sekund \n", sewtime - sswtime);

    printNumsSito(isprime, n, m);
    free(isprime);
}

void sito_rownolegle(long long n, long long m)
{
	clock_t spstart, spstop,ppstart,ppstop;
	

	double sswtime, sewtime;
    //volatile

    
    char* isprime = (char*)calloc(m, sizeof(char));

    isprime[0]=1;
    isprime[1]=1;


	sswtime = omp_get_wtime();
	spstart = clock();

    long long i =0;
    omp_set_num_threads(Th_num);

    long long to = sqrt(m);

    #pragma omp parallel for schedule(dynamic,1) 
    for (i =1; i<=to;i++)
    {
        if(!isprime[i])
            for(long long j = i*2; j<m; j+=i)
                isprime[j]=1;
    }


	spstop = clock();
    sewtime = omp_get_wtime();
    
    long long prime_nums = 0;

    for(long long i = n;i<m;i++){
        if(!isprime[i])
            prime_nums++;
    }
    printf("%lld \n",prime_nums);

	printf("Czas procesorow przetwarzania rownoleglego  %f sekund \n", ((double)(spstop - spstart)/CLOCKS_PER_SEC));
	printf("Czas trwania obliczen rownoleglego - wallclock %f sekund \n",  sewtime-sswtime);	

    printNumsSito(isprime, n, m);
    free(isprime);
}

void sito_rownolegle_v2(long long n, long long m) {
    clock_t spstart, spstop, ppstart, ppstop;


    double sswtime, sewtime;

    char* isprime = (char*)calloc(m, sizeof(char));

    isprime[0] = 1;
    isprime[1] = 1;


    sswtime = omp_get_wtime();
    spstart = clock();

     
    omp_set_num_threads(Th_num);

    long long j;
    long long to = sqrt(m);

    #pragma omp parallel
    {
    for (long long i = 1; i <= to; i++)
    {
        if (!isprime[i]) {
            
            #pragma omp for schedule(static) nowait
            for (j = i * 2; j < m; j += i)
                isprime[j] = 1;
        }
    }
    }


    spstop = clock();
    sewtime = omp_get_wtime();

    long long prime_nums = 0;

    for (long long i = n; i < m; i++) {
        if (!isprime[i])
            prime_nums++;
    }
    printf("%lld \n", prime_nums);

    printf("Czas procesorow przetwarzania rownoleglego  %f sekund \n", ((double)(spstop - spstart) / CLOCKS_PER_SEC));
    printf("Czas trwania obliczen rownoleglego - wallclock %f sekund \n", sewtime - sswtime);

    printNumsSito(isprime, n, m);
    free(isprime);
}

void sito_rownolegle_blokowo(long long n, long long m)
{
    clock_t spstart, spstop, ppstart, ppstop;

    double sswtime, sewtime;


    char* isprime = (char*)calloc(m, sizeof(char));

    isprime[0] = 1;
    isprime[1] = 1;


    sswtime = omp_get_wtime();
    spstart = clock();

    omp_set_num_threads(Th_num);
    #pragma omp parallel
    {
        int id = omp_get_thread_num();
        int slice = m / omp_get_num_threads()+1;

        int from = id * slice;
        int to = from + slice;
        if (to > m)
            to = m;


        for (long long i = 0; i <= sqrt(to); i++)
        {
            if (!isprime[i])
            {
                for (long long j = max(i*2,(from/i)*i); j < m; j += i)
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

    for (long long i = 0; i < m; i++) {
        if (!isprime[i]) {
            prime_nums++;
        }
    }
    printf("%d \n", prime_nums);

    printf("Czas procesorow przetwarzania rownoleglego  %f sekund \n", ((double)(spstop - spstart) / CLOCKS_PER_SEC));
    printf("Czas trwania obliczen rownoleglego - wallclock %f sekund \n", sewtime - sswtime);

    printNumsSito(isprime, n, m);
    free(isprime);
}

void sito_rownolegle_blokowo_v2(long long n, long long m)
{
    clock_t spstart, spstop, ppstart, ppstop;

    double sswtime, sewtime;

    char* isprime = (char*)calloc(m, sizeof(char));


    isprime[0] = 1;
    isprime[1] = 1;


    sswtime = omp_get_wtime();
    spstart = clock();


    omp_set_num_threads(Th_num);
    #pragma omp parallel
    {
        int id = omp_get_thread_num();
        int slice = m / omp_get_num_threads() + 1;

        int from = id * slice;
        int to = from + slice;
        if (to > m)
            to = m;

        long long i;

        for (i = 0; i <= sqrt(to); i++)
        {
            if (isprime[i]==0)
            {
                if (i < from)
                    i--;
                else
                    isprime[i] = 2;
            }
            
            if (isprime[i] == 2)
            {
                for (long long j = max(i * 2, (from / i) * i); j < m; j += i)
                {
                    if (j >= from)
                        isprime[j] = 1;
                }
            }

        }

        for (; i < to; i++)
        {
            if (isprime[i] == 0)
            {
                isprime[i] = 2;
            }
        }
    }


    spstop = clock();
    sewtime = omp_get_wtime();

    int prime_nums = 0;

    for (long long i = 0; i < m; i++) {
        if (isprime[i]==2) {
            prime_nums++;
        }
    }
    printf("%d \n", prime_nums);

    printf("Czas procesorow przetwarzania rownoleglego  %f sekund \n", ((double)(spstop - spstart) / CLOCKS_PER_SEC));
    printf("Czas trwania obliczen rownoleglego - wallclock %f sekund \n", sewtime - sswtime);

    printNumsSito(isprime, n, m);
    free(isprime);
}


void dzielenie_sekwecyjnie(long long min, long long max) {
    //clock_t spstart, spstop;
    //double sswtime, sewtime;

    //1 - prime, 2 - not a prime
    char* primes;

    //SEKWENCYJNIE
    primes = (char*)calloc(max - min, sizeof(char));
    //sswtime = omp_get_wtime();
    //spstart = clock();

    if (min == 2) {
        primes[0] = 1;
        primes[1] = 1;
    } 
    else if (min == 3) {
        primes[0] = 1;
    }

    int mod, iterConst;
    mod = min % 6;
    iterConst = mod;
    if (iterConst == 0) {
        iterConst = 6;
    }
    else if (iterConst == 1) {
        iterConst = 7;
    }
        
    for (long long i = 6 - iterConst; i < max - min + 1; i+=6) {
        if (i - 1 >= 0) {
            primes[i - 1] = 1;
            if ((i - 1 + min) % 2 == 0 || (i - 1 + min) % 3 == 0) {
                primes[i - 1] = 2;
                continue;
            }
            for (long long j = 6; j - 1 <= (long long)ceil(sqrt(i - 1 + min)); j+=6) {
                if ((i - 1 + min) % (j - 1) == 0 || (i - 1 + min) % (j + 1) == 0) {
                    primes[i - 1] = 2;
                    break;
                }
            }
        }
        if (i < max - min - 1) {
            primes[i + 1] = 1;
            if ((i + 1 + min) % 2 == 0 || (i + 1 + min) % 3 == 0) {
                primes[i + 1] = 2;
                continue;
            }
            for (long long j = 6; j - 1 <= (long long)ceil(sqrt(i + 1 + min)); j += 6) {
                if ((i + 1 + min) % (j - 1) == 0 || (i + 1 + min) % (j + 1) == 0) {
                    primes[i + 1] = 2;
                    break;
                }
            }
        }
    }

    //spstop = clock();
    //sewtime = omp_get_wtime();

    //printPrimesNum(primes, max, min);
    //printPrimes(primes, max, min);
    free(primes);

    //printf("Czas procesorow przetwarzania sekwencyjnego  %f sekund \n", ((double)(spstop - spstart) / CLOCKS_PER_SEC));
    //printf("Czas trwania obliczen sekwencyjnego - wallclock %f sekund \n", sewtime - sswtime);
}

void dzielenie_rownolegle(long long min, long long max) {
    //clock_t ppstart, ppstop;
    //double pswtime, pewtime;

    //1 - prime, 2 - not a prime, 0 -not checked or not qualify to check
    char* primes;

    //RÓWNOLEGLE
    primes = (char*)calloc(max - min, sizeof(char));
    //pswtime = omp_get_wtime();
    //ppstart = clock();

    if (min == 2) {
        primes[0] = 1;
        primes[1] = 1;
    }
    else if (min == 3) {
        primes[0] = 1;
    }

    int mod, iterConst;
    mod = min % 6;
    iterConst = mod;
    if (iterConst == 0) {
        iterConst = 6;
    }
    else if (iterConst == 1) {
        iterConst = 7;
    }

    omp_set_num_threads(Th_num);
    #pragma omp parallel
    {
        long long i, j;
    #pragma omp for schedule(guided, 40)
        for (i = 6 - iterConst; i < max - min + 1; i += 6) {
            if (i - 1 >= 0) {
                primes[i - 1] = 1;
                if ((i - 1 + min) % 2 == 0 || (i - 1 + min) % 3 == 0) {
                    primes[i - 1] = 2;
                    continue;
                }
                for (j = 6; j - 1 <= (long long)ceil(sqrt(i - 1 + min)); j += 6) {
                    if ((i - 1 + min) % (j - 1) == 0 || (i - 1 + min) % (j + 1) == 0) {
                        primes[i - 1] = 2;
                        break;
                    }
                }
            }
            if (i + 1 < max - min) {
                primes[i + 1] = 1;
                if ((i + 1 + min) % 2 == 0 || (i + 1 + min) % 3 == 0) {
                    primes[i + 1] = 2;
                    continue;
                }
                for (j = 6; j - 1 <= (long long)ceil(sqrt(i + 1 + min)); j += 6) {
                    if ((i + 1 + min) % (j - 1) == 0 || (i + 1 + min) % (j + 1) == 0) {
                        primes[i + 1] = 2;
                        break;
                    }
                }
            }
        }
    }

    //ppstop = clock();
    //pewtime = omp_get_wtime();

    //printPrimes(primes, max, min);
    //printPrimesNum(primes, max, min);

    free(primes);

    //printf("Czas procesorow przetwarzania rownoleglego  %f sekund \n", ((double)(ppstop - ppstart) / CLOCKS_PER_SEC));
    //printf("Czas trwania obliczen rownoleglego - wallclock %f sekund \n", pewtime - pswtime);
}

int main(int argc, char* argv[])
{
    long long n = 2;
    long long m = 50000000;


    //sito_sekwencyjnie(n,m);
    //sito_rownolegle(n,m);
    //sito_rownolegle_v2(n, m);
    //sito_rownolegle_blokowo(n,m);
    //sito_rownolegle_blokowo_v2(n, m);
    //dzielenie_sekwecyjnie(n, m);
    //dzielenie_rownolegle_OLD(n, m);
    //dzielenie_rownolegle(n, m);

    return 0;
}

//
// gcc one.c -lm -fopenmp