#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <pthread.h>
#include <time.h>



void main(int argc, char* argv[]) {

    char* eptr;
    unsigned long long int y2;
    if ((argc > 2) || (argc <=1)){
        printf("not enough parameters: exiting \n");
        exit(-1);
    }
    unsigned long long int n = strtoull(argv[1],&eptr,10);     // VALUE OF N
    printf("Value of N : %lld \n",n);


    time_t st;
    time_t ed;
    st = time(NULL);
    clock_t t;
    

    unsigned long long int i = 0;
    unsigned long long int sum = 0;

    t = clock();

    for (i=0; i <n; i++){
        sum += i;
    }

    printf("sum = %lld \n", sum);

    t = clock() - t;
    double time_taken = ((double)t)/CLOCKS_PER_SEC; // in seconds
 
    printf("%f seconds to execute \n", time_taken);
    
    ed =time(NULL);
    
    printf("%f Real time taken to execute\n",difftime(ed,st));

}