#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <semaphore.h>
#include <math.h>
#include <time.h>


unsigned long long int addvalues(unsigned long long int s, unsigned long long int e){
    unsigned long long int i;
    unsigned long long int sum=0;               //FUNCTION CREATED TO CALCULATE SUM WITH LIMITS
    for (i=s; i <= e; i++){
    sum += i;
        
}
return sum;
}



int total_task(int expo){
    int p = 1;
    int i = 0;
    while(i < expo){                            //FUNCTION TO CALCULATE THE NUMBER OF PROCESSES CREATED
    p = p*2;                                    //BY CALLING FORK() MANY TIMES
    i++;
    }
    p = (p/2)-1;
    return p;
}



int main(int argc, char* argv[]) {

    char* eptr;
    unsigned long long int y2;                                 //INITIALIZATION
    if ((argc > 3) || (argc <=2)){
        printf("not enough parameters: exiting \n");
        exit(-1);
    }

    int NUM_TASK = atoi(argv[1]); ;                             //STRING FUNCTION TO ACCESS VALUE
    printf("Value of NUM_TASK : %d \n",NUM_TASK);

    unsigned long long int n =strtoull(argv[2],&eptr,10);       //STRING FUNCTION TO ACCESS VALUE
    printf("Value of N : %lld \n",n);
    n = n-1;

    clock_t t;
    clock_t tp;
                                                        // SETTING TIME AND CLOCK FOR
    time_t st;                                          //CPU TIME AND REAL TIME
    time_t ed;
    st = time(NULL);

    

    unsigned long long int start =0;
    unsigned long long int sum = 0;                     //INITIALIZATION
    unsigned long long int temp = 0;
    unsigned long long int temp2=0;
    unsigned long long int temp3=0;
    unsigned long long int temp4=0;
    unsigned long long int rd;
    double time_taken;

    int prcs = NUM_TASK;
    int TOT_Task = 0;
    TOT_Task = total_task(prcs); 
    pid_t pid[prcs];                                // CREATING PID ID
    

    int pi =0;
    int x = 0;
    int fd[2];
    int fdp[2];

    pipe(fd);
    pipe(fdp);                                      //CREATING PIPE FOR DATA TRANSFER

    t = clock();

    for(pi;pi<prcs;pi++){
        pid[pi] = fork();
        if (pid[pi] == 0){
            n=abs(((n+start)/2));                   //TO DIVIDE THE WORKLOAD EQUALLY FOR PARENT AND
        }                                           //CHILD PROCESS CREATED BY GIVING THE RANGE LIMIT
        else if (pid[pi] > 0){
            wait(NULL);
            start=abs(((n+start)/2)+1);
        }
    }

    
    for(pi=0;pi<prcs;pi++){
        if (pid[pi] > 0){                           //TO KEEP TRACK OF THE MAIN PARENT
            x += 1;
        }
    }


    if (pid[prcs-1] == 0){
        tp = clock();
        sum =addvalues(start,n);                //CHILD PROCESS WORK LOAD CODE
        close(fd[0]);
        write(fd[1],&sum,sizeof(unsigned long long int));       //SENDING DATA THROUGH PIPE
        tp = clock()-tp;
        time_taken = ((double)t)/CLOCKS_PER_SEC;
        printf("%f CPU time taken to execute a child process \n", time_taken);

        exit(0);
    }



    else if ((pid[prcs-1] > 0) &&(x < prcs)){
        tp = clock();
        sum =addvalues(start,n);
        wait(NULL);
        close(fd[1]);                               //INTERMEDIATE PARENTS WORK
        rd = 0;
            while (1){
            read(fd[0],&temp3,sizeof(unsigned long long int));
            rd = temp3;             
                                                    //READING DATA FROM CHILD
            if (rd != 0){
                
                break;
            } 
            }
        
        temp4 = sum + temp3;            //ADDING THE VALUES FROM CHILD TO ITSELF
        close(fdp[0]);                                          //SENDING DATA TO MAIN PARENT
        write(fdp[1],&temp4,sizeof(unsigned long long int));
        tp = clock()-tp;
        time_taken = ((double)t)/CLOCKS_PER_SEC;
        printf("%f CPU time taken to execute a sub parent/child process \n", time_taken);
    }
    

    else if ((pid[prcs-1] > 0) && (x == prcs)){
        tp = clock();                                   //MAIN PARENT WORK
        wait(NULL);
        sum =addvalues(start,n);
        close(fd[1]);
        rd = 0;
            while (1){                                          //READING DATA FROM IMMEDIATE CHILD
            read(fd[0],&temp3,sizeof(unsigned long long int));
            rd = temp3;
            if (rd != 0){
                break;
                } 
            }
        
        temp4 = sum + temp3;
        unsigned long long int fin_sum =0;
        fin_sum = temp4;
        if (prcs <=2){
            prcs = prcs -1;
            }
       for (int i=0;i<TOT_Task;i++){
            wait(NULL);
            close(fdp[1]);
            rd = 0;
            while (1){
            read(fdp[0],&temp,sizeof(unsigned long long int));         //READING DATA FROM OTHER PARENTS
            rd = temp;
            if (rd != 0){
                break;
            } 
            }
            fin_sum += temp;
            
    }  printf("Final SUM - %lld\n",fin_sum);                    //CALCULATING FINAL SUM
    tp = clock()-tp;
        time_taken = ((double)t)/CLOCKS_PER_SEC;
        printf("%f CPU time taken to execute a Main Parent process \n", time_taken);
    t = clock() - t;
    time_taken = ((double)t)/CLOCKS_PER_SEC;                    //CALCULATIN TIMES
    
    printf("%f CPU time taken to execute and finish from Fork called \n", time_taken);
    ed =time(NULL);
    
    printf("%f Real time taken to execute\n",difftime(ed,st));
}   
    return 0;}

