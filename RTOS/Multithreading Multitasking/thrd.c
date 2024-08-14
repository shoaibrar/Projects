#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <semaphore.h>
#include <math.h>
#include <pthread.h>
#include <time.h>




void* Taskread(void* r){
    
    clock_t tp;
    tp = clock();
    double time_taken;

    unsigned long long int* sum = (unsigned long long int*)malloc(sizeof(unsigned long long int));
    * sum = 0;

    unsigned long long int s;
    unsigned long long int* range = (unsigned long long int*)r;
    for (unsigned long long int i= range[0]; i <= range[1]; i++){
    s += i;                                 
        }                                       //tHREAD tASK TO CALCULATE SUM WITH LIMITS
    *((unsigned long long int*)sum) += s;

    tp = clock() - tp;
    time_taken = ((double)tp)/CLOCKS_PER_SEC; // in seconds

    printf("%f CPU time taken for thread to execute \n", time_taken);
pthread_exit((void*)sum);
}


unsigned long long int main(int argc, char* argv[]) {

    char* eptr;
    unsigned long long int y2;
    if ((argc > 3) || (argc <=2)){
        printf("not enough parameters: exiting \n");        //INITIALIZATION
        exit(-1);
    }                           

    int NUM_THREAD = atoi(argv[1]);                     //STRING FUNCTION TO ACCESS VALUE
    printf("Value of NUM_THREAD : %d \n",NUM_THREAD);

    unsigned long long int n = strtoull(argv[2],&eptr,10);   //STRING FUNCTION TO ACCESS VALUE
    printf("Value of N : %lld \n",n);
    n = n-1;

    time_t st;
    time_t ed;
    st = time(NULL);
    clock_t t;


    double time_taken;
    unsigned long long int fin_sum = 0;             //INITIALIZATION OF VARIABLES
    unsigned long long int s[2];
    s[0] = 0;
    s[1] = n;
    unsigned long long int r[NUM_THREAD][2];
    unsigned long long int nxt_ptr = 0;
    unsigned long long int* receive = NULL;

    unsigned long long int div_task;            //CALCULATING THE INTERVALS
    div_task = (n/NUM_THREAD);

    pthread_t thread_id[NUM_THREAD];            //VARIABLE FOR THREAD ID
    
    nxt_ptr = s[0];

    t = clock();

    for(unsigned long long int i=0;i<NUM_THREAD;i++){
        r[i][0] = nxt_ptr;
        
        if(r[i][0]>= s[1]){
            r[i][0] = s[1];
        }
                                            //ASSIGNING EACH THREAD ITS LIMIT TO CALCULATE
        r[i][1] = r[i][0]+div_task;
        
        if(r[i][1] > s[1]){
            r[i][1] = s[1];
        }
        

        pthread_create(&thread_id[i], NULL, Taskread, (void*)&r[i]);    //CREATING THREAD AND PASSING 
        nxt_ptr = r[i][1]+1;                                            // THE LIMITS

    }
    
    for(unsigned long long int i = 0; i< NUM_THREAD; i++){
        pthread_join(thread_id[i],(void**)&receive);                //READING THE SUM FROM EACH THREAD
        fin_sum += *receive;
    }

    printf("FINAL SUM = %lld \n", fin_sum);                 //CALCULATING FINAL SUM
    
    t = clock() - t;
    time_taken = ((double)t)/CLOCKS_PER_SEC; // in seconds

    printf("%f CPU time taken to execute \n", time_taken);      //CALCULATING TIMES
    ed =time(NULL);
    
    printf("%f Real time taken to execute\n",difftime(ed,st));
    
return 0;
}