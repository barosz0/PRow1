#include <stdio.h>
#include <time.h>
#include <math.h>
#include <string.h>
#include <omp.h>
#include <stdlib.h>
#include <conio.h>

# define Th_num 6

void printFirstLast(char* primes, long long max, long long min) {
    long long primeNums = 0;
    long long first = -1, last;
    for (long long i = 0; i < max - min; i++) {
        if (primes[i] == 1) {
            primeNums++;
            if (first == -1)
                first = i + min;
            last = i + min;
        }
    }
    printf("%lld %lld\n", first, last);
    printf("Liczb pierwszych: %lld\n", primeNums);
}


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

void printPrimesSito(char* primes, long long max, long long min)
{
    int count = 0;
    for (long long i = 0; i <= max - min; i++) {
        if (primes[i] == 0) {
            count++;
            if (count == 10) {
                printf("%lld\n", i + min);
                count = 0;
            }
            else
                printf("%lld ", i + min);
        }
    }
}

void sito_sekwencyjnie(long long n, long long m)
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

    long long to = sqrt(m);

    for (long long i = 0; i < to; i++)
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
            //printf("%d, ",i);
            prime_nums++;
    }
    printf("%lld \n", prime_nums);

    printPrimesSito(isprime, m, n);

    free(isprime);

    printf("Czas procesorow przetwarzania sekwencyjnego  %f sekund \n", ((double)(spstop - spstart) / CLOCKS_PER_SEC));
    printf("Czas trwania obliczen sekwencyjnych - wallclock %f sekund \n", sewtime - sswtime);
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

    #pragma omp parallel for schedule(static)
    for (i =1; i<to;i++)
    {
        //printf("%d ma %d\n",omp_get_thread_num(), i);
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
}

void sito_rownolegle_v2(long long n, long long m) {
    clock_t spstart, spstop, ppstart, ppstop;


    double sswtime, sewtime;
    //volatile


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
    for (long long i = 1; i < to; i++)
    {
        //printf("%d ma %d\n",omp_get_thread_num(), i);
        if (!isprime[i]) {
            
            #pragma omp for schedule(static)
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
}

void sito_rownolegle_blokowo(long long n, long long m)
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

    //int id;



    omp_set_num_threads(Th_num);
    //#pragma omp parallel for schedule(static)
   // for ( id = 0; id < omp_get_num_threads(); id++)
    #pragma omp parallel
    {
        int id = omp_get_thread_num();
        int slice = m / omp_get_num_threads()+1;

        int from = id * slice;
        int to = from + slice;
        if (to > m)
            to = m;
        //printf("%d: %d %d\n", id, from, to);
        // int to = m - i*slice;
        // int from = to - slice;

        // if (from < 0)
        //     from = 0;



        for (long long i = 0; i < sqrt(to); i++)
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
            //printf("%d, ", i);
            prime_nums++;
        }
    }
    printf("%d \n", prime_nums);

    printf("Czas procesorow przetwarzania rownoleglego  %f sekund \n", ((double)(spstop - spstart) / CLOCKS_PER_SEC));
    printf("Czas trwania obliczen rownoleglego - wallclock %f sekund \n", sewtime - sswtime);
}

void sito_rownolegle_blokowo_v2(long long n, long long m)
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

    //int id;



    omp_set_num_threads(Th_num);
    //#pragma omp parallel for schedule(static)
   // for ( id = 0; id < omp_get_num_threads(); id++)
#pragma omp parallel
    {
        int id = omp_get_thread_num();
        int slice = m / omp_get_num_threads() + 1;

        int from = id * slice;
        int to = from + slice;
        if (to > m)
            to = m;
        //printf("%d: %d %d\n", id, from, to);
        // int to = m - i*slice;
        // int from = to - slice;

        // if (from < 0)
        //     from = 0;

        long long i;

        for (i = 0; i < sqrt(to); i++)
        {
            if (isprime[i]==0)
            {
                if (i < from)
                  
                    while (!isprime[i]) {
                        i--;

                    }
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
            //printf("%d, ", i);
            prime_nums++;
        }
    }
    printf("%d \n", prime_nums);

    printf("Czas procesorow przetwarzania rownoleglego  %f sekund \n", ((double)(spstop - spstart) / CLOCKS_PER_SEC));
    printf("Czas trwania obliczen rownoleglego - wallclock %f sekund \n", sewtime - sswtime);
}


void dzielenie_sekwecyjnie(long long min, long long max) {
    clock_t spstart, spstop;
    double sswtime, sewtime;

    //1 - prime, 2 - not a prime
    char* primes;

    //SEKWENCYJNIE
    primes = (char*)calloc(max - min, sizeof(char));
    sswtime = omp_get_wtime();
    spstart = clock();

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
        
    for (long long i = 6 - iterConst; i < max - min + 1 /* || i < max - min - 1*/; i+=6) {
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

    spstop = clock();
    sewtime = omp_get_wtime();

    //printPrimesNum(primes, max, min);
    //printPrimes(primes, max, min);
    printFirstLast(primes, max, min);
    free(primes);

    printf("Czas procesorow przetwarzania sekwencyjnego  %f sekund \n", ((double)(spstop - spstart) / CLOCKS_PER_SEC));
    printf("Czas trwania obliczen sekwencyjnego - wallclock %f sekund \n", sewtime - sswtime);
}

void dzielenie_rownolegle(long long min, long long max) {
    clock_t ppstart, ppstop;
    double pswtime, pewtime;

    //1 - prime, 2 - not a prime
    char* primes;

    //RÓWNOLEGLE
    primes = (char*)calloc(max - min + 1, sizeof(char));

    pswtime = omp_get_wtime();
    ppstart = clock();
    long long minp, maxp;
    omp_set_num_threads(Th_num);
    #pragma omp parallel private(minp, maxp)
    {
        minp = min;
        maxp = max;
        long long i;
        #pragma omp for schedule(guided, 40)
        for (i = 0; i <= maxp - minp; i++) {
            primes[i] = 1;
            for (long long j = 2; j <= (long long)ceil(sqrt(i + minp)); j++) {
                if (((i + minp) % j == 0) && (i + minp != 2)) {
                    primes[i] = 2;
                    break;
                }
            }
        }
    }

    ppstop = clock();
    pewtime = omp_get_wtime();

    //printPrimes(primes, max, min);
    //printPrimesNum(primes, max, min);
    printFirstLast(primes, max, min);

    free(primes);

    printf("Czas procesorow przetwarzania rownoleglego  %f sekund \n", ((double)(ppstop - ppstart) / CLOCKS_PER_SEC));
    printf("Czas trwania obliczen rownoleglego - wallclock %f sekund \n", pewtime - pswtime);
}

///*
void dzielenie_rownolegle2(long long min, long long max) {
    clock_t ppstart, ppstop;
    double pswtime, pewtime;

    //1 - prime, 2 - not a prime
    char* primes;

    //RÓWNOLEGLE
    primes = (char*)calloc(max - min + 1, sizeof(char));
    pswtime = omp_get_wtime();
    ppstart = clock();

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
#pragma omp parallel firstprivate(min, max)
    {
        long long i, j;
#pragma omp for schedule(guided, 40)
        for (i = 6 - iterConst; i < max - min + 1 /* || i < max - min - 1*/; i += 6) {
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

    ppstop = clock();
    pewtime = omp_get_wtime();

    //printPrimes(primes, max, min);
    //printPrimesNum(primes, max, min);
    printFirstLast(primes, max, min);

    free(primes);

    printf("Czas procesorow przetwarzania rownoleglego  %f sekund \n", ((double)(ppstop - ppstart) / CLOCKS_PER_SEC));
    printf("Czas trwania obliczen rownoleglego - wallclock %f sekund \n", pewtime - pswtime);
}
//*/

int main(int argc, char* argv[])
{
    long long n = 2;
    long long m = 100000;


    //sito_sekwencyjnie(n,m);
    //sito_rownolegle(n,m);
    //sito_rownolegle_v2(n, m);
    //sito_rownolegle_blokowo(n,m);
    //sito_rownolegle_blokowo_v2(n, m);
    //dzielenie_sekwecyjnie(n, m);
    //dzielenie_rownolegle(n, m);
    //dzielenie_rownolegle2(n, m);

    /*
    int min, max;
    srand(time(NULL));
    
    int iStart = 0, iStop = 6, jStart = 0, jStop = 6;


    for (int i = iStart; i < jStop; i++) {
        for (int j = jStart; j < jStop; j++) {
            min = (rand()/6)*6+i;
            max = ((min + rand()) / 6) * 6 + j;
            printf("i: %d, j: %d\n", i, j);
            printf("min: %d, max: %d\n", min, max);
            //dzielenie_sekwecyjnie(min, max);
            dzielenie_rownolegle(min, max);
            dzielenie_rownolegle2(min, max);
        }
    }
    //*/
    return 0;
}

//
// gcc one.c -lm -fopenmp