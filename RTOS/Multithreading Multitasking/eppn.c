#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <semaphore.h>
#include <math.h>
#include <time.h>
#include <string.h>





int main(int argc, char* argv[100]) {
	unsigned long long int s = 0;                                  // EXECUTABLE FILE FOR POPEN METHOD
	unsigned long long int e = 0;
    
    char* eptr;
    char n[100];

    s = strtoull(argv[1],&eptr,10);                         //STRING FUNCTIONS USED TO CONVERT DATA TYPE
    e = strtoull(argv[2],&eptr,10);                         //INTO A ACCESSIBLE ONE
    
    unsigned long long int sum = 0;
    unsigned long long int i = 0;
    for(i = s;i<=e;i++){
    	sum +=i;
    }
    printf("%lld", sum);                                    //CONVERTING TO STRING FOR DATA TRANSFER
    return 0;
    

}