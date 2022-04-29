#include <stdio.h>
#include <time.h>
#include <math.h>
#include <string.h>
#include <omp.h>


int main(int argc, char* argv[])
{
	clock_t spstart, spstop,ppstart,ppstop;
	
    int n = 0, m = 1000000;

	double sswtime, sewtime;
//volatile

    int tab[m];

    memset(tab,0,sizeof(tab));
    tab[0]=1;
    tab[1]=1;

//SEKWENCYJNIE
	sswtime = omp_get_wtime();
	spstart = clock();

	for (int i =0; i<m;i++)
    {

        if(tab[i])
            {
                continue;   
            }

        int j = 2;
        int num = i;
        while(num *j < m)
        {
            //printf("%d kasuje %d \n",i, num *j);
            tab[num*j] = 1;
            j++;
        }
    }

	spstop = clock();
    sewtime = omp_get_wtime();
   
    int prime_nums = 0;

    for(int i = 0;i<m;i++){
        if(tab[i]==0)
            //printf("%d, ",i);
            prime_nums++;
    }
    printf("%d \n",prime_nums);

	printf("Czas procesorów przetwarzania sekwencyjnego  %f sekund \n", ((double)(spstop - spstart)/CLOCKS_PER_SEC));
	printf("Czas trwania obliczen sekwencyjnych - wallclock %f sekund \n",  sewtime-sswtime);
	return 0;
}

// gcc first.c -lm -fopenmp -o first.out