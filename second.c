#include <stdio.h>
#include <time.h>
#include <math.h>
#include <string.h>
#include <omp.h>
#include <stdlib.h>



int main(int argc, char* argv[])
{
	clock_t spstart, spstop,ppstart,ppstop;
	
    int n = 0, m = 1000000;

	double sswtime, sewtime;
//volatile

    
    char* isprime = (char*)calloc(m, sizeof(char));

    isprime[0]=1;
    isprime[1]=1;


	sswtime = omp_get_wtime();
	spstart = clock();

    int i =0;
    omp_set_num_threads(4);
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
	return 0;
}
