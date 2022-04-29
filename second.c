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

    memset(tab,0,m);
    tab[0]=1;
    tab[1]=1;

//SEKWENCYJNIE
	sswtime = omp_get_wtime();
	spstart = clock();
    #pragma omp parallel shared(tab)
    {
        #pragma omp parallel for
        for (int i =2; i<m/2;i++)
        {
            //printf("%d %d\n", i , omp_get_thread_num());
            if(tab[i])
                continue;

            int j = 2;
            int num = i;

            
            while(num *j < m)
            {
                if(tab[i])
                    break;
                // printf("%d \n", num *j);
               
                tab[num*j] = 1;
                j++;
            }

            
        }
    }

	spstop = clock();
    sewtime = omp_get_wtime();
   
    // for(int i = 0;i<m;i++){
    //     if(tab[i]==0)
    //         printf("%d ", i);
    // }
    printf("\n");

	printf("Czas procesorów przetwarzania rownoleglego  %f sekund \n", ((double)(spstop - spstart)/CLOCKS_PER_SEC));
	printf("Czas trwania obliczen rownoleglego - wallclock %f sekund \n",  sewtime-sswtime);
	return 0;
}
