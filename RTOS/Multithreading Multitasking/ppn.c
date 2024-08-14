#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <semaphore.h>
#include <math.h>
#include <time.h>
#include <string.h>

int main(int argc, char* argv[]) {
    
    int y;
    char* eptr;
    unsigned long long int y2;
    if ((argc > 3) || (argc <=2)){
        printf("not enough parameters: exiting \n");
        exit(-1);
    }
    int NUM_TASK = atoi(argv[1]);                             //STRING FUNCTION TO ACCESS VALUE
    printf("Value of NUM_TASK : %d \n",NUM_TASK);


    unsigned long long int n= strtoull(argv[2],&eptr,10);           //STRING FUNCTION TO ACCESS VALUE
    printf("Value of N : %lld \n",n);
    n = n-1;

    time_t st;
    time_t ed;
    st = time(NULL);                                                // SETTING TIME AND CLOCK FOR
    clock_t t;                                                         //CPU TIME AND REAL TIME

    
    FILE* fp[NUM_TASK];                                         //CREATING FILE POINTER

    
    unsigned long long int receive =0;
    double time_taken;
    char arg[100];
    char temp[100] = "";
    unsigned long long int tempp;                               //INITIALIZATION
    
    unsigned long long int fin_sum = 0;
    unsigned long long int s[2];
    s[0] = 0;
    s[1] = n;
    unsigned long long int r[NUM_TASK][2];
    unsigned long long int nxt_ptr = 0;

    strcpy(arg,"./test ");

    unsigned long long int div_task;
    div_task = (n/NUM_TASK);                        //CALCULATING THE INTERVALS
    
    nxt_ptr = s[0];
    t = clock();
    for(int i=0;i<NUM_TASK;i++){
        temp[0] = '\0';
        
        strcpy(arg,"./test ");                      //USING STRING FUNCTION TO PASS VAULES
        r[i][0] = nxt_ptr;
        
        if(r[i][0]>= s[1]){
            r[i][0] = s[1];                         //ASSIGNING EACH POPEN ITS LIMIT TO CALCULATE
        }

        r[i][1] = r[i][0]+div_task;
        
        if(r[i][1] > s[1]){
            r[i][1] = s[1];
        }
        
        sprintf(temp," %lld %lld",r[i][0],r[i][1]);
        strcat(arg,temp);
        fp[i] = popen(arg,"r");                 //CREATING A TASK AND PASSING THE LIMITS

        nxt_ptr = r[i][1]+1;
    }

    for(int i = 0; i< NUM_TASK; i++){
        fscanf(fp[i],"%lld",&tempp);            //READING THE VALUES FROM EACH TASK
        fin_sum += tempp;

    }

    printf("FINAL SUM = %lld \n", fin_sum);         //FINAL SUM
   
    t = clock() - t;
    time_taken = ((double)t)/CLOCKS_PER_SEC; // in seconds
 
    printf("%f CPU time taken to execute the program \n", time_taken);
    ed =time(NULL);                             
                                            //CALCULATING THE TIMES
    printf("%f Real time taken to execute\n",difftime(ed,st));

    return 0;
}
